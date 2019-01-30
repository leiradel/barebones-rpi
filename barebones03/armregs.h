#ifndef ARMREGS_H__
#define ARMREGS_H__

#include <stdint.h>

/****************************************************************************\
| Stack Pointer                                                              |
\****************************************************************************/

inline void set_sp(const uint32_t x) {
  __asm volatile("mov sp, %[value]\n" : : [value] "r" (x) : "sp");
}

#endif /* ARMREGS_H__ */
