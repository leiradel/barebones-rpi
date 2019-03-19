#include "syscalls.h"

static int sys_close(int file) {
  (void)file;
  return -1;
}

static void sys_exit(int rc) {
  (void)rc;
  // TODO: Exit to main.
  while (1) {
    // Nothing.
  }
}

static int sys_fstat(int file, struct stat* pstat) {
  (void)file;
  (void)pstat;
  return -1;
}

static int sys_getpid(void) {
  return 1;
}

static int sys_gettimeofday(struct timeval* tv, void* tz) {
  (void)tv;
  (void)tz;
  return -1;
}

static int sys_isatty(int fd) {
  (void)fd;
  return 0;
}

static int sys_kill(int pid, int sig) {
  (void)pid;
  (void)sig;
  return -1;
}

static int sys_link(const char* oldpath, const char* newpath) {
  (void)oldpath;
  (void)newpath;
  return -1;
}

static off_t sys_lseek(int file, off_t offset, int whence) {
  (void)file;
  (void)offset;
  (void)whence;
  return (off_t)-1;
}

static int sys_open(const char* filename, int flags, va_list args) {
  (void)filename;
  return -1;
}

static ssize_t sys_read(int fd, void* buf, size_t count) {
  (void)fd;
  (void)buf;
  (void)count;
  return -1;
}

static clock_t sys_times(struct tms* buffer) {
  (void)buffer;
  return (clock_t)-1;
}

static int sys_unlink(const char* pathname) {
  (void)pathname;
  return -1;
}

static ssize_t sys_write(int fd, const void* buf, size_t count) {
  (void)fd;
  (void)buf;
  (void)count;
  return -1;
}

syscalls_t g_syscalls = {
  sys_close,
  sys_exit,
  sys_fstat,
  sys_getpid,
  sys_gettimeofday,
  sys_isatty,
  sys_kill,
  sys_link,
  sys_lseek,
  sys_open,
  sys_read,
  sys_times,
  sys_unlink,
  sys_write
};
