#include "arm.h"

#include <stdint.h>

uint32_t g_baseio, g_busalias;

void (*mem_isb)(void) = 0;
void (*mem_dmb)(void) = 0;
void (*mem_dsb)(void) = 0;

static inline uint32_t read32(uint32_t const address) {
  uint32_t result;

  __asm volatile(
    "ldr %[result], [%[address]]\n"
    : [result] "=r" (result)
    : [address] "r" (address)
  );

  return result;
}

// https://www.raspberrypi.org/forums/viewtopic.php?p=582004#p582004

static void isb_v6(void) {
  // mcr p15, 0, r3, c7, c5, #4
  __asm volatile(".long 0xee073f95\n" : : : "memory");
}

static void isb_v7_8(void) {
  // isb
  __asm volatile(".long 0xf57ff06f\n" : : : "memory");
}

static void dmb_v6(void) {
  // mcr p15, 0, r3, c7, c10, #5
  __asm volatile(".long 0xee073fba\n" : : : "memory");
}

static void dmb_v7_8(void) {
  // dmb
  __asm volatile(".long 0xf57ff05f\n" : : : "memory");
}

static void dsb_v6(void) {
  // mcr p15, 0, r3, c7, c10, #4
  __asm volatile(".long 0xee073f9a\n" : : : "memory");
}

static void dsb_v7_8(void) {
  // dsb
  __asm volatile(".long 0xf57ff04f\n" : : : "memory");
}

int memio_init(void) {
  // Select the correct barrier functions depending on the CPU. We could've
  // used this method to detect the peripheral's base I/O address...
  uint32_t const midr    = get_midr();
  uint32_t const part_no = midr & MIDR_PRIMARY_PART_NO;

  if (part_no == MIDR_PART_NO_ARM1176JZF_S) {
    mem_isb = isb_v6;
    mem_dmb = dmb_v6;
    mem_dsb = dsb_v6;
  }
  else if (part_no == MIDR_PART_NO_CORTEX_A7 ||
           part_no == MIDR_PART_NO_CORTEX_A53) {
    mem_isb = isb_v7_8;
    mem_dmb = dmb_v7_8;
    mem_dsb = dsb_v7_8;
  }
  else {
    return -1;
  }

  // Test for peripherals at 0x20000000.
  if (read32(UINT32_C(0x20215010)) == UINT32_C(0x61757830)) {
    g_baseio = UINT32_C(0x20000000);
    // ARM physical addresses go to the L2 cache coherent bus alias.
    g_busalias = UINT32_C(0x40000000);
  }
  // Test for peripherals at 0x3f000000.
  else if (read32(UINT32_C(0x3f215010)) == UINT32_C(0x61757830)) {
    g_baseio = UINT32_C(0x3f000000);
    // ARM physical addresses go to the uncached bus alias.
    g_busalias = UINT32_C(0xC0000000);
  }
  else {
    mem_dmb();
    return -1;
  }

  mem_dmb();
  return 0;
}
