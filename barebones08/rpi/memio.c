#include "armregs.h"

#include <stdint.h>

uint32_t g_baseio, g_busalias;

void (*mem_isb)(void) = 0;
void (*mem_dmb)(void) = 0;
void (*mem_dsb)(void) = 0;

static inline uint32_t read32(const uint32_t address) {
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
  __asm volatile("mcr p15, 0, %[zero], c7, c5, #4\n" : : [zero] "r" (0) : "memory");
}

static void isb_v7_8(void) {
  __asm volatile(".long 0xf57ff06f\n"); // isb
}

static void dmb_v6(void) {
  __asm volatile("mcr p15, 0, %[zero], c7, c10, #5\n" : : [zero] "r" (0) : "memory");
}

static void dmb_v7_8(void) {
  __asm volatile(".long 0xf57ff05f\n"); // dmb
}

static void dsb_v6(void) {
  __asm volatile("mcr p15, 0, %[zero], c7, c10, #4\n" : : [zero] "r" (0) : "memory");
}

static void dsb_v7_8(void) {
  __asm volatile(".long 0xf57ff04f\n"); // dsb
}

int memio_init(void) {
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
    return -1;
  }

  const uint32_t midr = get_midr();
  const uint32_t part_no = midr & MIDR_PRIMARY_PART_NO;

  // Select the correct barrier functions depending on the CPU. We could've
  // used this method to detect the peripheral's base I/O address...
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

  return 0;
}
