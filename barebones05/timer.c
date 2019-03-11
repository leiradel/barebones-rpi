#include "memio.h"

#define CS  0x00
#define CLO 0x04
#define CHI 0x08
#define C0  0x0c
#define C1  0x10
#define C2  0x14
#define C3  0x18

#define BASE_ADDR (g_baseio + UINT32_C(0x00003000))

uint32_t timer_high(void) {
  return mem_read32(BASE_ADDR + CHI);
}

uint32_t timer_low(void) {
  return mem_read32(BASE_ADDR + CLO);
}

uint64_t timer(void) {
  const uint32_t clo = BASE_ADDR + CLO;
  const uint32_t chi = BASE_ADDR + CHI;
  uint32_t high, low;

  do {
    high = mem_read32(chi);
    low = mem_read32(clo);
  }
  while (mem_read32(chi) != high);
  
  return (uint64_t)high << 32 | low;
}

void timer_wait_until(const uint64_t t1) {
  while (timer() < t1) {
    // Empty.
  }
}
