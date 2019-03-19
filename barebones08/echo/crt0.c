#include "memio.h"
#include "syscalls.h"

#include <stdint.h>
#include <string.h>

void mainCRTStartup(uint32_t const syscalls) {
  extern uint8_t __bss_start, __bss_end;
  memset(&__bss_start, 0, &__bss_end - &__bss_start);

  memio_init();

  extern syscalls_t g_syscalls;
  g_syscalls = *(syscalls_t*)syscalls;

  extern int main(int, char**);
  static char* argv[] = {"main"};
  (void)main(sizeof(argv) / sizeof(argv[0]), argv);
}
