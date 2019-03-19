#include "mbox.h"
#include "memio.h"

#include <stdint.h>

#define READ0   0x00
#define PEEK0   0x10
#define SENDER0 0x14
#define STATUS0 0x18
#define CONFIG0 0x1c
#define WRITE1  0x20
#define PEEK1   0x30
#define SENDER1 0x34
#define STATUS1 0x38
#define CONFIG1 0x3c

#define TAGS 8

#define BASE_ADDR (g_baseio + UINT32_C(0x0000b880))

int mbox_send(void* msg) {
  // Wait for the mailbox to become empty.
  while (1) {
    uint32_t const value = mem_read32(BASE_ADDR + STATUS1);

    if ((value & UINT32_C(0x80000000)) == 0) {
      // Mailbox empty, exit loop.
      break;
    }
  }

  mem_dmb();

  // Send message to channel 8: tags (ARM to VC).
  uint32_t const msgaddr = (mem_arm2vc((uint32_t)msg) & ~15) | TAGS;
  mem_write32(BASE_ADDR + WRITE1, msgaddr);

  // Wait for the response.
  while (1) {
    while (1) {
      uint32_t const value = mem_read32(BASE_ADDR + STATUS0);
      
      if ((value & UINT32_C(0x40000000)) == 0) {
        // Response arrived, exit loop.
        break;
      }
    }

    uint32_t const value = mem_read32(BASE_ADDR + READ0);

    if ((value & 15) == TAGS) {
      // Correct channel, exit loop.
      break;
    }
  }

  mem_dmb();

  if (((mbox_msgheader_t*)msg)->code == UINT32_C(0x80000000)) {
    return 0; // Success!
  }

  return -1; // Ugh...
}
