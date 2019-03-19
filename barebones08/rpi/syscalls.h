#ifndef SYSCALLS_H__
#define SYSCALLS_H__

#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/times.h>
#include <stdarg.h>

typedef struct {
  int     (*close)(int file);
  void    (*exit)(int rc);
  int     (*fstat)(int file, struct stat* pstat);
  int     (*getpid)(void);
  int     (*gettimeofday)(struct timeval* tv, void* tz);
  int     (*isatty)(int fd);
  int     (*kill)(int pid, int sig);
  int     (*link)(const char* oldpath, const char* newpath);
  off_t   (*lseek)(int file, off_t offset, int whence);
  int     (*open)(const char* filename, int flags, va_list args);
  ssize_t (*read)(int fd, void* buf, size_t count);
  clock_t (*times)(struct tms* buffer);
  int     (*unlink)(const char* pathname);
  ssize_t (*write)(int fd, const void* buf, size_t count);
}
syscalls_t;

#endif /* SYSCALLS_H__ */
