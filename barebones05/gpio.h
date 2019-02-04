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

void gpio_select(const unsigned pin, const gpio_function_t mode);
void gpio_set(const unsigned pin, const int high);

#endif /* GPIO_H__ */
