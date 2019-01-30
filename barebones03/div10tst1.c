#include <stdio.h>

static int test(unsigned magic, unsigned shift) {
  unsigned i, a, b;

  for (i = 0; i < 54; i++) {
    a = i / 10;
    b = (i * magic) >> shift;

    if (a != b) {
      return 0;
    }
  }

  return 1;
}

int main() {
  unsigned magic, shift;
  int ok;

  for (shift = 0; shift < 29; shift++) {
    magic = ((1 << shift) + 5) / 10;
    ok = test(magic, shift);
    
    if (!ok) {
      /* Why does this work??? */
      ok = test(++magic, shift);
    }

    printf("(pin * 0x%08xU) >> %2u => %d\n", magic, shift, ok);
  }

  return 0;
}
