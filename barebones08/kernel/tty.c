#include "tty.h"
#include "aux.h"
#include "memio.h"

#include <stdint.h>

#define RX_BUFFER_SIZE 128

static uint8_t s_readbuf[RX_BUFFER_SIZE];
static uint32_t s_head, s_tail;

static uint32_t get_head(void) {
  return mem_read32((uint32_t)&s_head);
}

static void set_head(uint32_t const new_head) {
  mem_write32((uint32_t)&s_head, new_head);
}

static uint32_t get_tail(void) {
  return mem_read32((uint32_t)&s_tail);
}

static void set_tail(uint32_t const new_tail) {
  mem_write32((uint32_t)&s_tail, new_tail);
}

static void newchar(uint8_t const k) {
  // Add the character to the read ring buffer.
  uint32_t const head     = get_head();
  uint32_t const new_head = (head + 1) % RX_BUFFER_SIZE;

  if (new_head != get_tail()) {
    // There's space available for the new character.
    s_readbuf[head] = (uint8_t)k;
    set_head(new_head);
  }
}

void tty_init(void) {
  s_head = s_tail = 0;
  uart_init(115200, newchar);
}

int tty_canread(void) {
  return get_head() != get_tail();
}

char tty_read(void) {
  while (!tty_canread()) {
    // nothing
  }

  // There's data available in the receive ring buffer, return it.
  uint32_t const tail     = get_tail();
  uint8_t  const k        = s_readbuf[tail];
  uint32_t const new_tail = (tail + 1) % RX_BUFFER_SIZE;
  set_tail(new_tail);

  return (char)k;
}

void tty_write(char const k) {
  uart_write(k, 0);
}
