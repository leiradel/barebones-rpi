#include "syscalls.h"

// Extend the heap space at the end of the .bss section.
void* _sbrk(intptr_t increment) {
  extern char __heap_start;
  static const char* start = &__heap_start;
  const char* prev = start;

  // Keep memory returned by _sbrk 16-byte aligned.
  increment = (increment + 15) & ~15;
  start += increment;

  return (void*)prev;
}

syscalls_t g_syscalls;

int _close(int file) {
  return g_syscalls.close(file);
}

void _exit(int rc) {
  g_syscalls.exit(rc);

  while (1) {
    // nothing
  }
}

int _fstat(int file, struct stat* pstat) {
  return g_syscalls.fstat(file, pstat);
}

int _getpid(void) {
  return g_syscalls.getpid();
}

int _gettimeofday(struct timeval* tv, void* tz) {
  return g_syscalls.gettimeofday(tv, tz);
}

int _isatty(int fd) {
  return g_syscalls.isatty(fd);
}

int _kill(int pid, int sig) {
  return g_syscalls.kill(pid, sig);
}

int _link(const char* oldpath, const char* newpath) {
  return g_syscalls.link(oldpath, newpath);
}

off_t _lseek(int file, off_t offset, int whence) {
  return g_syscalls.lseek(file, offset, whence);
}

int _open(const char* filename, int flags, ...) {
  va_list args;
  va_start(args, flags);
  int ret = g_syscalls.open(filename, flags, args);
  va_end(args);
  return ret;
}

ssize_t _read(int fd, void* buf, size_t count) {
  return g_syscalls.read(fd, buf, count);
}

clock_t _times(struct tms* buffer) {
  return g_syscalls.times(buffer);
}

int _unlink(const char* pathname) {
  return g_syscalls.unlink(pathname);
}

ssize_t _write(int fd, const void* buf, size_t count) {
  return g_syscalls.write(fd, buf, count);
}
