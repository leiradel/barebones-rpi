#ifndef GPIO_H__
#define GPIO_H__

typedef enum {
  GPIO_INPUT      = 0,
  GPIO_OUTPUT     = 1,
  GPIO_FUNCTION_0 = 4,
  GPIO_FUNCTION_1 = 5,
  GPIO_FUNCTION_2 = 6,
  GPIO_FUNCTION_3 = 7,
  GPIO_FUNCTION_4 = 3,
  GPIO_FUNCTION_5 = 2
}
gpio_function_t;

typedef enum {
  GPIO_PULL_OFF  = 0,
  GPIO_PULL_DOWN = 1,
  GPIO_PULL_UP   = 2
}
gpio_pull_t;

void gpio_select(unsigned const pin, gpio_function_t const mode);
void gpio_set(unsigned const pin, int const high);
void gpio_setpull(unsigned const pin, gpio_pull_t const pull);

#endif /* GPIO_H__ */
