#ifndef LED_H__
#define LED_H__

extern void (*led_set)(const int on);

int led_init(void);

#endif /* LED_H__ */
