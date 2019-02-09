#include "gpio.h"
#include "memio.h"

#include <stdint.h>

#define GPFSEL0   0x00
#define GPFSEL1   0x04
#define GPFSEL2   0x08
#define GPFSEL3   0x0c
#define GPFSEL4   0x10
#define GPFSEL5   0x14
#define GPSET0    0x1c
#define GPSET1    0x20
#define GPCLR0    0x28
#define GPCLR1    0x2c
#define GPLEV0    0x34
#define GPLEV1    0x38
#define GPEDS0    0x40
#define GPEDS1    0x44
#define GPREN0    0x4c
#define GPREN1    0x50
#define GPFEN0    0x58
#define GPFEN1    0x5c
#define GPHEN0    0x64
#define GPHEN1    0x68
#define GPLEN0    0x70
#define GPLEN1    0x74
#define GPAREN0   0x7c
#define GPAREN1   0x80
#define GPAFEN0   0x88
#define GPAFEN1   0x8c
#define GPPUD     0x94
#define GPPUDCLK0 0x98
#define GPPUDCLK1 0x9c

// The GPIO base displacement from the base IO address.
#define BASE_ADDR (g_baseio + UINT32_C(0x00200000))

void gpio_select(const unsigned pin, const gpio_function_t mode) {
  // The register index starting at GPFSEL0.
  const unsigned index = (pin * 0xcdU) >> 11;
  // Amount to shift to get to the required pin bits.
  const unsigned shift = (pin - index * 10) * 3;

  // Calculate the GPFSEL register that contains the configuration for
  // the pin.
  const uint32_t gpfsel = BASE_ADDR + GPFSEL0 + index * 4;

  // Read the register.
  const uint32_t value = mem_read32(gpfsel);

  // Set the desired function for the pin.
  const uint32_t masked = value & ~(UINT32_C(7) << shift);
  const uint32_t fsel = masked | mode << shift;

  // Write the value back to the register.
  mem_write32(gpfsel, fsel);
}

void gpio_set(const unsigned pin, const int high) {
  // The register index starting at GPSET0 or GPCLR0.
  const unsigned index = pin >> 5;
  // The bit in the registers to set or clear the pin.
  const uint32_t bit = UINT32_C(1) << (pin & 31);

  if (high) {
    // Write the bit to GPSEL to set the pin high.
    mem_write32(BASE_ADDR + GPSET0 + index * 4, bit);
  }
  else {
    // Write the bit to GPCLR to set the pin low.
    mem_write32(BASE_ADDR + GPCLR0 + index * 4, bit);
  }
}

static void wait(unsigned count) {
  // Spend CPU cycles.
  while (count-- != 0) {
    __asm volatile("");
  }
}

void gpio_setpull(const unsigned pin, const gpio_pull_t pull) {
  const unsigned index = pin >> 5;
  const uint32_t bit = UINT32_C(1) << (pin & 31);

  // Set GPPUD to the desired pull up/down.
  mem_write32(BASE_ADDR + GPPUD, pull);
  // Spend cycles.
  wait(150);

  // Program the pull up/down mode using GPPUDCLK.
  if (index == 0) {
    // Set the pin bit in GPPUDCLK0.
    mem_write32(BASE_ADDR + GPPUDCLK0, bit);
    // Spend cycles.
    wait(150);
    // Clear the bit.
    mem_write32(BASE_ADDR + GPPUDCLK0, 0);
  }
  else {
    // Same thing, but use GPPUDCLK1.
    mem_write32(BASE_ADDR + GPPUDCLK1, bit);
    wait(150);
    mem_write32(BASE_ADDR + GPPUDCLK1, 0);
  }
}
