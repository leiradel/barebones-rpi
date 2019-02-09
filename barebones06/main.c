#include <stdint.h>

#include "timer.h"
#include "led.h"

void main(const uint32_t machine_type, const uint32_t atags_addr) {
  uint64_t time = timer();

  while (1) {
    led_set(1);
    timer_waituntil(time += 100000);
    led_set(0);
    timer_waituntil(time += 900000);
  }
}
