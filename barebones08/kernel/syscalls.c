#include "syscalls.h"
#include "tty.h"

#include <setjmp.h>
#include <signal.h>
#include <errno.h>
#include <stdint.h>

jmp_buf g_exitaddr;

static int sys_close(int file) {
  (void)file;
  errno = EBADF;
  return -1;
}

static void sys_exit(int rc) {
  longjmp(g_exitaddr, rc);
}

static int sys_fstat(int file, struct stat* pstat) {
  (void)file;
  (void)pstat;
  errno = ENOENT;
  return -1;
}

static int sys_getpid(void) {
  return 1;
}

static int sys_gettimeofday(struct timeval* tv, void* tz) {
  (void)tv;
  (void)tz;
  errno = EINVAL;
  return -1;
}

static int sys_isatty(int fd) {
  (void)fd;
  errno = ENOTTY;
  return 0;
}

static int sys_kill(int pid, int sig) {
  if (pid == sys_getpid() && sig == SIGINT) {
    longjmp(g_exitaddr, 130);
  }

  errno = EPERM;
  return -1;
}

static int sys_link(const char* oldpath, const char* newpath) {
  (void)oldpath;
  (void)newpath;
  errno = ENOENT;
  return -1;
}

static off_t sys_lseek(int file, off_t offset, int whence) {
  (void)file;
  (void)offset;
  (void)whence;
  errno = EBADF;
  return (off_t)-1;
}

static int sys_open(const char* filename, int flags, va_list args) {
  (void)filename;
  (void)flags;
  (void)args;
  errno = ENOENT;
  return -1;
}

static ssize_t sys_read(int fd, void* buf, size_t count) {
  if (fd == STDIN_FILENO) {
    uint8_t* chars = (uint8_t*)buf;

    for (size_t i = 0; i < count; i++) {
      uint8_t const k = tty_read();

      if (k == 3) {
        // Ctrl-C
        tty_write('^');
        tty_write('C');
        tty_write('\r');
        tty_write('\n');
        sys_kill(sys_getpid(), SIGINT);
      }

      if (k == '\r') {
        *chars++ = '\n';
        tty_write('\r');
        tty_write('\n');
        break;
      }
      else if (k == '\b') {
        if (chars > (uint8_t*)buf) {
          chars--;
          tty_write('\b');
          tty_write(' ');
          tty_write('\b');
        }
      }
      else {
        *chars++ = k;
        tty_write(k);
      }
    }

    return (ssize_t)(chars - (uint8_t*)buf);
  }

  errno = EBADF;
  return -1;
}

static clock_t sys_times(struct tms* buffer) {
  (void)buffer;
  errno = EFAULT; // Not really accurate, but it's the only one.
  return (clock_t)-1;
}

static int sys_unlink(const char* pathname) {
  (void)pathname;
  errno = ENOENT;
  return -1;
}

static ssize_t sys_write(int fd, const void* buf, size_t count) {
  if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
    const uint8_t* chars = (const uint8_t*)buf;

    for (size_t i = 0; i < count; i++) {
      if (*chars == '\n') {
        tty_write('\r');
      }

      tty_write(*chars++);
    }

    return (ssize_t)count;
  }

  errno = EBADF;
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
