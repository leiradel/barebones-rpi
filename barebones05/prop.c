#include "mbox.h"

#include <stdint.h>

uint32_t prop_get_revision(void) {
  typedef struct {
    mbox_msgheader_t header;
    mbox_tagheader_t tag;
    
    union {
      // No request.
      struct {
        uint32_t revision;
      }
      response;
    }
    value;

    mbox_msgfooter_t footer;
  }
  message_t;

  message_t msg __attribute__((aligned(16)));

  msg.header.size = sizeof(msg);
  msg.header.code = 0;
  msg.tag.id = UINT32_C(0x00010002); // Get board revision.
  msg.tag.size = sizeof(msg.value);
  msg.tag.code = 0;
  msg.footer.end = 0;

  if (mbox_send(&msg) != 0) {
    return 0;
  }

  return msg.value.response.revision;
}
