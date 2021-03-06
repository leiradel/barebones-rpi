#include <stdint.h>

uint32_t g_baseio, g_busalias;

static inline uint32_t read32(const uint32_t address) {
  uint32_t result;

  __asm volatile(
    "ldr %[result], [%[address]]\n"
    : [result] "=r" (result)
    : [address] "r" (address)
  );

  return result;
}

int memio_init(void) {
  // Test for peripherals at 0x20000000.
  if (read32(UINT32_C(0x20215010)) == UINT32_C(0x61757830)) {
    g_baseio = UINT32_C(0x20000000);
    // ARM physical addresses go to the L2 cache coherent bus alias.
    g_busalias = UINT32_C(0x40000000);
    return 0;
  }

  // Test for peripherals at 0x3f000000.
  if (read32(UINT32_C(0x3f215010)) == UINT32_C(0x61757830)) {
    g_baseio = UINT32_C(0x3f000000);
    // ARM physical addresses go to the uncached bus alias.
    g_busalias = UINT32_C(0xC0000000);
    return 0;
  }

  return -1;
}
