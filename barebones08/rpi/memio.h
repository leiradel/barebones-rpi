#ifndef MEMIO_H__
#define MEMIO_H__

#include <stdint.h>

extern uint32_t const g_baseio;
extern uint32_t const g_busalias;

extern void (*mem_isb)(void);
extern void (*mem_dmb)(void);
extern void (*mem_dsb)(void);

inline uint32_t mem_read32(uint32_t const address) {
  uint32_t result;

  __asm volatile(
    "ldr %[result], [%[address]]\n"
    : [result] "=r" (result)
    : [address] "r" (address)
  );

  return result;
}

inline void mem_write32(uint32_t const address, uint32_t const value) {
  __asm volatile(
    "str %[value], [%[address]]\n"
    :
    : [address] "r" (address), [value] "r" (value)
  );
}

inline uint32_t mem_arm2vc(uint32_t const address) {
  return address | g_busalias;
}

inline uint32_t mem_vc2arm(uint32_t const address) {
  return address & ~g_busalias;
}

int memio_init(void);

#endif /* MEMIO_H__ */
