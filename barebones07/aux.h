#ifndef AUX_H__
#define AUX_H__

void uart_init(const unsigned baudrate);

int uart_canwrite(void);
int uart_write(const int k, unsigned timeout_us);

int uart_canread(void);
int uart_read(unsigned timeout_us);

#endif /* AUX_H__ */
