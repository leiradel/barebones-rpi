#include <unistd.h>
#include <stdint.h>

static void writechar(uint8_t const k) {
  static uint8_t const hex[] = "0123456789abcdef";

  uint8_t const space = ' ';
  write(STDOUT_FILENO, &space, 1);

  uint8_t const _100 = k / 100 + '0';
  write(STDOUT_FILENO, &_100, 1);

  uint8_t const _10 = (k - (_100 - '0') * 100) / 10 + '0';
  write(STDOUT_FILENO, &_10, 1);

  uint8_t const _1 = k % 10 + '0';
  write(STDOUT_FILENO, &_1, 1);

  write(STDOUT_FILENO, &space, 1);

  uint8_t const bs[] = "\\x";
  write(STDOUT_FILENO, bs, 2);

  write(STDOUT_FILENO, hex + ((k >> 4) & 15), 1);
  write(STDOUT_FILENO, hex + (k & 15), 1);

  uint8_t const nl[] = "\r\n";
  write(STDOUT_FILENO, nl, 2);
}

int main(int argc, const char* const argv[]) {
  (void)argc;
  (void)argv;

  while (1) {
    uint8_t k;
    read(STDIN_FILENO, &k, 1);
    writechar(k);

    if (k == 'x') {
      break;
    }
  }

  return 0;
}
