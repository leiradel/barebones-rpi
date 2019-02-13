#ifndef MEMIO_H__
#define MEMIO_H__

#include <stdint.h>

extern const uint32_t g_baseio;
extern const uint32_t g_busalias;

extern void (*mem_isb)(void);
extern void (*mem_dmb)(void);
extern void (*mem_dsb)(void);

inline uint32_t mem_read32(const uint32_t address) {
  uint32_t result;
  mem_dmb();

  __asm volatile(
    "ldr %[result], [%[address]]\n"
    : [result] "=r" (result)
    : [address] "r" (address)
  );

  mem_dmb();
  return result;
}

inline void mem_write32(const uint32_t address, const uint32_t value) {
  mem_dmb();

  __asm volatile(
    "str %[value], [%[address]]\n"
    :
    : [address] "r" (address), [value] "r" (value)
  );

  mem_dmb();
}

inline uint32_t mem_arm2vc(const uint32_t address) {
  return address | g_busalias;
}

inline uint32_t mem_vc2arm(const uint32_t address) {
  return address & ~g_busalias;
}

int memio_init(void);

#endif /* MEMIO_H__ */
