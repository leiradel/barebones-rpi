#ifndef AUX_H__
#define AUX_H__

#include <stdint.h>

typedef void (*uart_newchar_t)(uint8_t const k);

void uart_init(unsigned const baudrate, uart_newchar_t const newchar);
void uart_clear(void);

int uart_canwrite(void);
int uart_write(int const k, unsigned const timeout_us);

int uart_canread(void);
int uart_read(unsigned const timeout_us);

#endif /* AUX_H__ */
