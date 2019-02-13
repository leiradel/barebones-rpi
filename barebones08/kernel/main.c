#include "aux.h"
#include "led.h"
#include "memio.h"

typedef void (*main_t)(const uint32_t, const uint32_t);

static void writestr(const char* str) {
  while (*str != 0) {
    uart_write(*str++, 0);
  }
}

static void writebyte(int byte) {
  static const char hex[] = "0123456789abcdef";
  uart_write(hex[byte >> 4], 0);
  uart_write(hex[byte & 15], 0);
}

static void writeword(int word) {
  writebyte(word >> 8);
  writebyte(word & 255);
}

static void writeln(void) {
  writestr("\r\n");
}

static int wait(void) {
  static int led = 0;
  int k;

  do {
    led_set(led ^= 1);
  }
  while ((k = uart_read(500000)) < 0);

  return k & 0xff;
}

static int read(void) {
  int k = uart_read(1000000);

  if (k < 0) {
    writestr("Error, read timeout\r\n");
  }

  writestr("Read char ");

  if (k > 0x20 && k < 0x7f) {
    uart_write(k, 0);
  }
  else {
    writestr("\\x");
    writebyte(k);
  }

  writeln();

  return k;
}

static void skip(void) {
  static int led = 0;

  do {
    led_set(led ^= 1);
  }
  while (uart_read(1000000) >= 0);
}

static int hex(int digit) {
  if (digit >= '0' && digit <= '9') {
    return digit - '0';
  }
  else if (digit >= 'A' && digit <= 'F') {
    return digit - 'A' + 10;
  }
  else {
    writestr("Error, invalid hexa digit ");

    if (digit > 0x20 && digit < 0x7f) {
      uart_write(digit, 0);
    }
    else {
      writestr("\\x");
      writebyte(digit);
    }

    writeln();
    return -1;
  }
}

static int readbyte(void) {
  int digit1 = read();
  if (digit1 < 0) return -1;

  int digit0 = read();
  if (digit0 < 0) return -1;

  digit1 = hex(digit1);
  if (digit1 < 0) return -1;

  digit0 = hex(digit0);
  if (digit0 < 0) return -1;

  writestr("Read byte ");
  writebyte(digit1 << 4 | digit0);
  writeln();

  return digit1 << 4 | digit0;
}

static int readword(void) {
  int byte1 = readbyte();
  if (byte1 < 0) return -1;

  int byte0 = readbyte();
  if (byte0 < 0) return -1;

  writestr("Read word ");
  writebyte(byte1);
  writebyte(byte0);
  writeln();

  return byte1 << 8 | byte0;
}

void main(const uint32_t machine_type, const uint32_t atags_addr) {
  uart_init(115200);

  while (1) {
    writestr("Waiting for file\r\n");

    uint8_t* addr = 0;
    uint32_t entry = 0;
    int end = 0;

    while (!end) {
      uint8_t bytes[256];
      int k = wait();

      if (k == '\r') {
        k = read();

        if (k == '\n') {
          k = read();
        }
      }
      else if (k == '\n') {
        k = read();
      }

      if (k != ':') {
        writestr("Error, expecting :\r\n");
error:
        skip();
        break;
      }

      unsigned checksum = 0;

      int count = readbyte();
      if (count < 0) goto error;
      checksum += count;

      writestr("Record has "); writebyte(count); writestr(" bytes\r\n");

      int offset = readword();
      if (offset < 0) goto error;
      checksum += offset >> 8;
      checksum += offset & 0xff;

      writestr("Offset is "); writeword(offset); writeln();

      int type = readbyte();
      if (type < 0) goto error;
      checksum += type;

      writestr("Record type is "); writebyte(type); writeln();
      writestr("Reading data bytes\r\n");

      for (int i = 0; i < count; i++) {
        k = readbyte();
        if (k < 0) goto error;
        checksum += k;

        bytes[i] = k;
      }

      switch (type) {
        case 0x00: {
          for (int i = 0; i < count; i++) {
            (addr + offset)[i] = bytes[i];
          }

          break;
        }

        case 0x01: {
          end = 1;
          break;
        }

        case 0x02: {
          addr = 0;
          addr += (bytes[0] * 256 + bytes[1]) * 16;
          break;
        }

        case 0x03: {
          entry = (bytes[0] * 256 + bytes[1]) * 16;
          entry += bytes[2] * 256 + bytes[3];
          break;
        }

        case 0x04: {
          addr = 0;
          addr += (bytes[0] * 256 + bytes[1]) * 65536;
          break;
        }

        case 0x05: {
          entry = (bytes[0] * 256 + bytes[1]) * 65536;
          entry += bytes[2] * 256 + bytes[3];
          break;
        }

        default: {
          goto error;
        }
      }

      int cs = readbyte();
      if (cs < 0) goto error;
      if (cs != (-checksum & 0xff)) goto error;

      writestr("Read checksum "); writebyte(cs); writestr(" (calculated "); writebyte(-checksum & 0xff); writestr(")\r\n");

      if (end) {
        ((main_t)entry)(machine_type, atags_addr);
      }
    }
  }
}
