#ifndef GLOD_H__
#define GLOD_H__

#include <stdint.h>

#ifdef GLOD_MORSE_CODE
#define GLOD_RESET     0x0001599dU /* RES .-.  .  ...    */
#define GLOD_UNDEFINED 0x0005e7b5U /* UND ..-  -.  -..   */
#define GLOD_PREFETCH  0x0006767dU /* PRE .--.  .-.  .   */
#define GLOD_ABORT     0x003f95edU /* ABO .-  -...  ---  */
#define GLOD_EXITED    0x00016d79U /* EXI .  -..-  ..    */
#else
/* don't blink only once to avoid confusing people counting the blinks */
#define GLOD_RESET     2
#define GLOD_UNDEFINED 3
#define GLOD_SOFTWARE  4
#define GLOD_PREFETCH  5
#define GLOD_ABORT     6
#define GLOD_EXITED    7
#endif

void glod(uint32_t const code);

#endif /* GLOD_H__ */
