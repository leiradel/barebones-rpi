#include "aux.h"
#include "gpio.h"
#include "memio.h"
#include "timer.h"
#include "prop.h"
#include "isr.h"

#include <stdint.h>
#include <stddef.h>

#define AUX_IRQ         0x00
#define AUX_ENABLES     0x04
#define AUX_MU_IO_REG   0x40
#define AUX_MU_IER_REG  0x44
#define AUX_MU_IIR_REG  0x48
#define AUX_MU_LCR_REG  0x4c
#define AUX_MU_MCR_REG  0x50
#define AUX_MU_LSR_REG  0x54
#define AUX_MU_MSR_REG  0x58
#define AUX_MU_SCRATCH  0x5c
#define AUX_MU_CNTL_REG 0x60
#define AUX_MU_STAT_REG 0x64
#define AUX_MU_BAUD_REG 0x68

#define BASE_ADDR (g_baseio + UINT32_C(0x00215000))

static uart_newchar_t s_callback;

static void uart_read_isr(uint32_t const value) {
  (void)value;
  mem_dmb();

  while (1) {
    uint32_t const iir = mem_read32(BASE_ADDR + AUX_MU_IIR_REG);

    if ((iir & 1) == 1) {
      // No Mini UART interrupt pending, return.
      break;
    }
    
    if ((iir & 6) == 4) {
      // Character available, remove it from the receive FIFO.
      uint32_t const k = mem_read32(BASE_ADDR + AUX_MU_IO_REG) & 0xff;
      // Send it to user code.
      s_callback(k);
    }
  }

  mem_dmb();
}

void uart_init(unsigned const baudrate, uart_newchar_t const newchar) {
  // Get the clock used with the Mini UART.
  uint32_t const clock_rate = prop_getclockrate(PROP_CLOCK_CORE);

  // Set pins 14 and 15 to use with the Mini UART.
  gpio_select(14, GPIO_FUNCTION_5);
  gpio_select(15, GPIO_FUNCTION_5);

  // Turn pull up/down off for those pins.
  gpio_setpull(14, GPIO_PULL_OFF);
  gpio_setpull(15, GPIO_PULL_OFF);

  mem_dmb();

  // Enable only the Mini UART.
  mem_write32(BASE_ADDR + AUX_ENABLES, 1);
  // Disable receiving and transmitting while we configure the Mini UART.
  mem_write32(BASE_ADDR + AUX_MU_CNTL_REG, 0);
  // Turn off interrupts.
  mem_write32(BASE_ADDR + AUX_MU_IER_REG, 0);
  // Set data size to 8 bits.
  mem_write32(BASE_ADDR + AUX_MU_LCR_REG, 3);
  // Put RTS high.
  mem_write32(BASE_ADDR + AUX_MU_MCR_REG, 0);
  // Clear both receive and transmit FIFOs.
  mem_write32(BASE_ADDR + AUX_MU_IIR_REG, 0xc6);

  // Set the desired baudrate.
  uint32_t const divisor = clock_rate / (8 * baudrate) - 1;
  mem_write32(BASE_ADDR + AUX_MU_BAUD_REG, divisor);

  if (newchar != NULL) {
    // Install the IRQ handler and enable read interrupts.
    s_callback = newchar;
    isr_sethandler(ISR_IRQ, uart_read_isr);
    mem_write32(BASE_ADDR + AUX_MU_IER_REG, 5);
    isr_enablebasic(29);
  }

  // Enable receiving and transmitting.
  mem_write32(BASE_ADDR + AUX_MU_CNTL_REG, 3);
}

void uart_clear(void) {
  mem_dmb();
  mem_write32(BASE_ADDR + AUX_MU_IIR_REG, 0x06);
}

int uart_canwrite(void) {
  uint32_t const value = mem_read32(BASE_ADDR + AUX_MU_LSR_REG);
  mem_dmb();
  return (value & 0x20) != 0;
}

int uart_write(int const k, unsigned const timeout_us) {
  if (timeout_us == 0) {
    // timeout_us == 0 means retry forever.
    while (!uart_canwrite()) {
      // nothing
    }
  }
  else {
    uint64_t const timeout = timer() + timeout_us;

    while (!uart_canwrite()) {
      if (timer() >= timeout) {
        return -1;
      }
    }
  }

  // There's space in the transmit FIFO, write the character
  mem_dmb();
  mem_write32(BASE_ADDR + AUX_MU_IO_REG, k & 0xff);
  return 0;
}

int uart_canread(void) {
  uint32_t const value = mem_read32(BASE_ADDR + AUX_MU_LSR_REG);
  mem_dmb();
  return value & 1;
}

int uart_read(unsigned const timeout_us) {
  if (timeout_us == 0) {
    // timeout_us == 0 means retry forever.
    while (!uart_canread()) {
      // nothing
    }
  }
  else {
    uint64_t const timeout = timer() + timeout_us;

    while (!uart_canread()) {
      if (timer() >= timeout) {
        return -1;
      }
    }
  }

  // There's data available in the receive FIFO, return it.
  int const k = mem_read32(BASE_ADDR + AUX_MU_IO_REG) & 0xff;
  mem_dmb();
  return k;
}
