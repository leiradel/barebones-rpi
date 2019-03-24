#ifndef TTY_H__
#define TTY_H__

#include <unistd.h>

void tty_init(void);
int  tty_canread(void);

char tty_read(void);
void tty_write(char k);

#endif /* TTY_H__ */
