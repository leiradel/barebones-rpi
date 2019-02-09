#ifndef GLOD_H__
#define GLOD_H__

#include <stdint.h>

#ifdef GLOD_MORSE_CODE
#define GLOD_RESET     UINT32_C(0x0001599d) /* RES .-.  .  ...    */
#define GLOD_UNDEFINED UINT32_C(0x0005e7b5) /* UND ..-  -.  -..   */
#define GLOD_PREFETCH  UINT32_C(0x0006767d) /* PRE .--.  .-.  .   */
#define GLOD_ABORT     UINT32_C(0x003f95ed) /* ABO .-  -...  ---  */
#define GLOD_EXITED    UINT32_C(0x00016d79) /* EXI .  -..-  ..    */
#define GLOD_RESERVED  UINT32_C(0x00ded675) /* FUK ..-.  ..-  -.- */
#else
/* don't blink only once to avoid confusing people counting the blinks */
#define GLOD_RESET     2
#define GLOD_UNDEFINED 3
#define GLOD_PREFETCH  4
#define GLOD_ABORT     5
#define GLOD_EXITED    6
#define GLOD_RESERVED  7 /* should never happen */
#endif

void glod(const uint32_t code);

#endif /* GLOD_H__ */
