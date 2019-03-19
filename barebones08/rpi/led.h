#ifndef LED_H__
#define LED_H__

extern void (*led_set)(int const on);

int  led_init(void);
void led_blink(unsigned const number, unsigned const repeats);

#endif /* LED_H__ */
