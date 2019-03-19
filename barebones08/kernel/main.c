#include "tty.h"
#include "led.h"
#include "timer.h"
#include "isr.h"
#include "glod.h"
#include "memio.h"
#include "arm.h"
#include "prop.h"
#include "syscalls.h"

#include <stdint.h>
#include <unistd.h>
#include <string.h>

static void writechar(char const k) {
  tty_write(&k, 1);
}

static void writestr(const char* str) {
  while (*str != 0) {
    writechar(*str++);
  }
}

static void writebyte(int const byte) {
  static const char hex[] = "0123456789abcdef";
  writechar(hex[byte >> 4]);
  writechar(hex[byte & 15]);
}

static void writeword(int const word) {
  writebyte((word >> 8) & 255);
  writebyte(word & 255);
}

static void writedword(uint32_t const dword) {
  writeword((dword >> 16) & 65535);
  writeword(dword & 65535);
}

static void writeescchar(int const k) {
  if (k > 0x20 && k < 0x7f) {
    writechar(k);
  }
  else if (k < 0) {
    writestr("'-\\x");
    writebyte(-k);
  }
  else {
    writestr("\\x");
    writebyte(k);
  }
}

static int readchar(void) {
  uint64_t const timeout = timer() + 1000000;

  while (!tty_canread()) {
    if (timer() >= timeout) {
      return -1;
    }
  }

  uint8_t k;
  tty_read(&k, 1);
  return k;
}

static void wait(void) {
  while (!tty_canread()) {
    led_set(timer() & (512 * 1024));
  }
}

static void skip(void) {
  uint64_t us, timeout = timer() + 1000000;

  do {
    us = timer();
    led_set(us & (128 * 1024));

    if (tty_canread()) {
      uint8_t k;
      tty_read(&k, 1);
      timeout = us + 1000000;
    }
  }
  while (us < timeout);
}

static int hex(int const digit) {
  if (digit >= '0' && digit <= '9') {
    return digit - '0';
  }
  else if (digit >= 'A' && digit <= 'F') {
    return digit - 'A' + 10;
  }
  else {
    writestr("Error, invalid hexa digit ");
    writeescchar(digit);
    writestr("\r\n");
    return -1;
  }
}

static int readbyte() {
  int const digit1 = hex(readchar());
  if (digit1 < 0) return -1;

  int const digit0 = hex(readchar());
  if (digit0 < 0) return -1;

  return digit1 << 4 | digit0;
}

static int readword() {
  int const byte1 = readbyte();
  if (byte1 < 0) return -1;

  int const byte0 = readbyte();
  if (byte0 < 0) return -1;

  return byte1 << 8 | byte0;
}

static void skipeol() {
  int const k = readchar();

  if (k == '\r') {
    readchar();
  }
}

static uint32_t load_ihex(void) {
  uint8_t* addr = 0;
  uint32_t entry = 0;

  writestr("Waiting for file\r\n");
  wait();

  while (1) {
    uint8_t bytes[256];
    int const k = readchar();

    if (k != ':') {
      writestr("Error, expecting ':', found ");
      writeescchar(k);
      writestr("\r\n");

error:
      skip();
      return 0;
    }

    unsigned checksum = 0;

    int const count = readbyte();
    if (count < 0) goto error;
    checksum += count;

    int const offset = readword();
    if (offset < 0) goto error;
    checksum += offset >> 8;
    checksum += offset & 0xff;

    int const type = readbyte();
    if (type < 0) goto error;
    checksum += type;

    for (int i = 0; i < count; i++) {
      int const k = readbyte();
      if (k < 0) goto error;
      checksum += k;

      bytes[i] = k;
    }

    int const cs = readbyte();
    if (cs < 0) goto error;

    if (cs != (-checksum & 0xff)) {
      writestr("Wrong checksum ");
      writebyte(-checksum & 0xff);
      writestr(" (calculated ");
      writebyte(cs);
      writestr(")\r\n");
      goto error;
    }

    switch (type) {
      case 0x00: {
        memcpy(addr + offset, bytes, count);
        break;
      }

      case 0x01: {
        skipeol();
        return entry;
      }

      case 0x02: {
        addr = (uint8_t*)((bytes[0] * 256 + bytes[1]) * 16);
        
        writestr("Address is ");
        writedword((uint32_t)addr);
        writestr("\r\n");
        break;
      }

      case 0x03: {
        entry  = (bytes[0] * 256 + bytes[1]) * 16;
        entry += bytes[2] * 256 + bytes[3];
        
        writestr("Entry is ");
        writedword((uint32_t)entry);
        writestr("\r\n");
        break;
      }

      case 0x04: {
        addr = 0;
        addr += (bytes[0] * 256 + bytes[1]) * 65536;
        
        writestr("Address is ");
        writedword((uint32_t)addr);
        writestr("\r\n");
        break;
      }

      case 0x05: {
        entry  = (bytes[0] * 256 + bytes[1]) * 65536;
        entry += bytes[2] * 256 + bytes[3];

        writestr("Entry is ");
        writedword(entry);
        writestr("\r\n");
        break;
      }

      default: {
        goto error;
      }
    }

    skipeol(1000000);
  }
}

static void undhandler(uint32_t const pc) {
  writestr("Undefined instruction at ");
  writedword(pc);
  glod(GLOD_UNDEFINED);
}

static ssize_t sys_read(int fd, void* buf, size_t count) {
  if (fd == STDIN_FILENO) {
    uint8_t* chars = (uint8_t*)buf;

    for (size_t i = 0; i < count; i++) {
      while (!tty_canread()) {
        // nothing
      }

      // There's data available in the receive ring buffer, return it.
      uint8_t k;
      ssize_t const numread = tty_read(&k, 1);

      if (numread == 0) {
        // If tty_canread returns 1 and tty_read returns 0, a Ctrl-C
        // has been detected.
        return 0;
      }

      if (k == '\r') {
        *chars++ = '\n';
        const uint8_t nl[] = "\r\n";
        tty_write(nl, 2);
        break;
      }
      else if (k == '\b') {
        if (chars > (uint8_t*)buf) {
          chars--;
          uint8_t const bs[] = "\b \b";
          tty_write(bs, 3);
        }
      }
      else {
        *chars++ = k;
        tty_write(&k, 1);
      }
    }

    return (ssize_t)(chars - (uint8_t*)buf);
  }

  return -1;
}

static ssize_t sys_write(int fd, const void* buf, size_t count) {
  if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
    return tty_write(buf, count);
  }

  return -1;
}

void main() {
  isr_sethandler(ISR_UNDEFINED, undhandler);

  tty_init();

  extern syscalls_t g_syscalls;
  g_syscalls.read = sys_read;
  g_syscalls.write = sys_write;

  memrange_t const mem = prop_armmemory();
  uint32_t const sp = (mem.base + mem.size) & ~3;

  while (1) {
    uint32_t const entry = load_ihex();

    if (entry != 0) {
      writestr("Executing entry ");
      writedword(entry);
      writestr(" with sp ");
      writedword(sp);
      writestr("\r\n");

      run_with_sp(entry, sp, (uint32_t)&g_syscalls);
    }
  }
}
