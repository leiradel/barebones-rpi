#include "gpio.h"
#include "memio.h"
#include "timer.h"

#include <stdint.h>

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

void uart_init(const unsigned baudrate) {
  // Set pins 14 and 15 to use with the Mini UART.
  gpio_select(14, GPIO_FUNCTION_5);
  gpio_select(15, GPIO_FUNCTION_5);

  // Turn pull up/down off for those pins.
  gpio_setpull(14, GPIO_PULL_OFF);
  gpio_setpull(15, GPIO_PULL_OFF);

  // Enable only the Mini UART.
  mem_write32(BASE_ADDR + AUX_ENABLES, 1);
  // Turn off interrupts, we'll use polling for now.
  mem_write32(BASE_ADDR + AUX_MU_IER_REG, 0);
  // Disable receiving and transmitting while we configure the Mini UART.
  mem_write32(BASE_ADDR + AUX_MU_CNTL_REG, 0);
  // Set data size to 8 bits.
  mem_write32(BASE_ADDR + AUX_MU_LCR_REG, 3);
  // Put RTS high.
  mem_write32(BASE_ADDR + AUX_MU_MCR_REG, 0);
  // Clear both receive and transmit FIFOs.
  mem_write32(BASE_ADDR + AUX_MU_IIR_REG, 0x06);
  // Set the desired baudrate.
  const uint32_t divisor = 250000000 / (8 * baudrate) - 1;
  mem_write32(BASE_ADDR + AUX_MU_BAUD_REG, divisor);
  // Enable receiving and transmitting.
  mem_write32(BASE_ADDR + AUX_MU_CNTL_REG, 3);
}

int uart_canwrite(void) {
  const uint32_t value = mem_read32(BASE_ADDR + AUX_MU_LSR_REG);
  return (value & 0x20) != 0;
}

int uart_write(const int k, unsigned timeout_us) {
  if (timeout_us == 0) {
    // timeout_us == 0 means retry forever.
    while (!uart_canwrite()) {
      // nothing
    }
  }
  else {
    const uint64_t timeout = timer() + timeout_us;

    while (!uart_canwrite()) {
      if (timer() >= timeout) {
        return -1;
      }
    }
  }

  // There's space in the transmit FIFO, write the character
  mem_write32(BASE_ADDR + AUX_MU_IO_REG, k & 0xff);
  return 0;
}

int uart_canread(void) {
  const uint32_t value = mem_read32(BASE_ADDR + AUX_MU_LSR_REG);
  return value & 1;
}

int uart_read(unsigned timeout_us) {
  if (timeout_us == 0) {
    // timeout_us == 0 means retry forever.
    while (!uart_canread()) {
      // nothing
    }
  }
  else {
    const uint64_t timeout = timer() + timeout_us;

    while (!uart_canread()) {
      if (timer() >= timeout) {
        return -1;
      }
    }
  }

  // There's data available in the receive FIFO, return it.
  return mem_read32(BASE_ADDR + AUX_MU_IO_REG) & 0xff;
}
