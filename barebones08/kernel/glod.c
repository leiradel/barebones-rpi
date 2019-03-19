#include "glod.h"
#include "led.h"
#include "timer.h"

void glod(uint32_t const code) {
#ifdef GLOD_MORSE_CODE
  uint64_t time = timer();
  led_set(0);

  while (1) {
    uint32_t pattern = code;

    for (unsigned i = 0; i < 16; i++, pattern >>= 2) {
      unsigned const code = pattern & 3;

      switch (code) {
        case 0: /* end of sequence */
          goto out;
        
        case 2: /* space */
          timer_waituntil(time += 600000);
          break;

        case 1: /* dot */
        case 3: /* dash */
          led_set(1);
          timer_waituntil(time += code * 200000);
          led_set(0);
          timer_waituntil(time += 200000);
          break;
      }
    }
    
out: (void)0;
    timer_waituntil(time += 500000);
  }
#else
  uint64_t time = timer();

  while (1) {
    for (uint32_t i = 0; i < code; i++) {
      led_set(1);
      timer_waituntil(time += 200000);
      led_set(0);
      timer_waituntil(time += 200000);
    }

    timer_waituntil(time += 300000);
  }
#endif
}
