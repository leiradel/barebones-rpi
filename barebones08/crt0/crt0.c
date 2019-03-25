#include "syscalls.h"
#include "memio.h"
#include "led.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void _init(void) {}
void _fini(void) {}

void mainCRTStartup(uint32_t const syscalls) {
  extern uint8_t __bss_start, __bss_end;
  memset(&__bss_start, 0, &__bss_end - &__bss_start);

  extern syscalls_t* g_syscalls;
  g_syscalls = (syscalls_t*)syscalls;

  memio_init();
  led_init();

  extern void __libc_init_array(void);
  extern void __libc_fini_array(void);
  atexit(__libc_fini_array);
  __libc_init_array();

  extern int main(int, char**, char**);
  static char* argv[] = {"main"};
  static char* envp[] = {NULL};
  
  int const retcode = main(sizeof(argv) / sizeof(argv[0]), argv, envp);

  void __call_exitprocs(int, void*);
  __call_exitprocs(retcode, NULL);

  g_syscalls->exit(retcode);
}
