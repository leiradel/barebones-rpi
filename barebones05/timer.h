#ifndef TIMER_H__
#define TIMER_H__

#include <stdint.h>

uint32_t timer_high(void);
uint32_t timer_low(void);
uint64_t timer(void);

void timer_wait_until(const uint64_t t1);

#endif /* TIMER_H__ */
