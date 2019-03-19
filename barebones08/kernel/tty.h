#ifndef TTY_H__
#define TTY_H__

#include <unistd.h>

void tty_init(void);
int  tty_canread(void);

ssize_t tty_read(void* const buf, size_t const count);
ssize_t tty_write(const void* const buf, size_t const count);

#endif /* TTY_H__ */
