#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>

#include "aux.h"

// Extend the heap space at the end of the .bss section.
void* _sbrk(intptr_t increment) {
  extern char heap_start;
  static const char* start = &heap_start;
  const char* prev = start;

  // Keep memory returned by _sbrk 16-byte aligned.
  increment = (increment + 15) & ~15;
  start += increment;

  return (void*)prev;
}

// Write stdout and stderr to the UART.
ssize_t _write(int fd, const void* buf, size_t count) {
  if (fd == 1 || fd == 2) {
    const char* str = buf;

    for (size_t i = 0; i < count; i++) {
      if (*str == '\n') {
        uart_write('\r', 0);
      }

      uart_write(*str++, 0);
    }

    return count;
  }

  return 0;
}

// Read stdin from the UART.
ssize_t _read(int fd, void* buf, size_t count) {
  if (fd == 0) {
    char* str = buf;

    for (size_t i = 0; i < count; i++) {
      *str++ = uart_read(0);
    }

    return count;
  }

  return 0;
}

int _close(int fd) {
  (void)fd;
  return 0;
}

int _fstat(int fd, struct stat* buf) {
  (void)fd;
  (void)buf;
  return 0;
}

int _isatty(int fd) {
  (void)fd;
  return 1;
}

off_t _lseek(int fd, off_t offset, int whence) {
  (void)fd;
  (void)offset;
  (void)whence;
  return 0;
}
