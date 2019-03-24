#include "syscalls.h"
#include "tty.h"

#include <setjmp.h>
#include <signal.h>
#include <stdint.h>

jmp_buf g_exitaddr;

static int sys_close(int file) {
  (void)file;
  return -1;
}

static void sys_exit(int rc) {
  longjmp(g_exitaddr, rc);
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
  if (pid == sys_getpid() && sig == SIGINT) {
    longjmp(g_exitaddr, 130);
  }

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
