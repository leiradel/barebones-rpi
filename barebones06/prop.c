#include "prop.h"
#include "mbox.h"

#include <stdint.h>

uint32_t prop_revision(void) {
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

uint32_t prop_fwrev(void) {
  typedef struct {
    mbox_msgheader_t header;
    mbox_tagheader_t tag;
    
    union {
      // No request.
      struct {
        uint32_t firmware;
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
  msg.tag.id = UINT32_C(0x00000001); // Get firmware revision.
  msg.tag.size = sizeof(msg.value);
  msg.tag.code = 0;
  msg.footer.end = 0;

  if (mbox_send(&msg) != 0) {
    return 0;
  }

  return msg.value.response.firmware;
}

uint32_t prop_model(void) {
  typedef struct {
    mbox_msgheader_t header;
    mbox_tagheader_t tag;
    
    union {
      // No request.
      struct {
        uint32_t model;
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
  msg.tag.id = UINT32_C(0x00010001); // Get board model.
  msg.tag.size = sizeof(msg.value);
  msg.tag.code = 0;
  msg.footer.end = 0;

  if (mbox_send(&msg) != 0) {
    return 0;
  }

  return msg.value.response.model;
}

uint64_t prop_macaddr(void) {
  typedef struct {
    mbox_msgheader_t header;
    mbox_tagheader_t tag;
    
    union {
      // No request.
      struct {
        uint8_t addr[6];
        uint8_t pad[2];
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
  msg.tag.id = UINT32_C(0x00010003); // Get board MAC address.
  msg.tag.size = sizeof(msg.value);
  msg.tag.code = 0;
  msg.footer.end = 0;

  if (mbox_send(&msg) != 0) {
    return 0;
  }

  return (uint64_t)msg.value.response.addr[0] << 40
       | (uint64_t)msg.value.response.addr[1] << 32
       | (uint64_t)msg.value.response.addr[2] << 24
       | (uint64_t)msg.value.response.addr[3] << 16
       | (uint64_t)msg.value.response.addr[4] <<  8
       | (uint64_t)msg.value.response.addr[5];
}

uint64_t prop_serial(void) {
  typedef struct {
    mbox_msgheader_t header;
    mbox_tagheader_t tag;
    
    union {
      // No request.
      struct {
        uint64_t serial;
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
  msg.tag.id = UINT32_C(0x00010004); // Get board serial.
  msg.tag.size = sizeof(msg.value);
  msg.tag.code = 0;
  msg.footer.end = 0;

  if (mbox_send(&msg) != 0) {
    return 0;
  }

  return msg.value.response.serial;
}

memrange_t prop_armmemory(void) {
  typedef struct {
    mbox_msgheader_t header;
    mbox_tagheader_t tag;
    
    union {
      // No request.
      struct {
        uint32_t base;
        uint32_t size;
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
  msg.tag.id = UINT32_C(0x00010005); // Get ARM memory.
  msg.tag.size = sizeof(msg.value);
  msg.tag.code = 0;
  msg.footer.end = 0;

  memrange_t range;

  if (mbox_send(&msg) != 0) {
    range.base = range.size = 0;
  }
  else {
    range.base = msg.value.response.base;
    range.size = msg.value.response.size;
  }

  return range;
}

memrange_t prop_vcmemory(void) {
  typedef struct {
    mbox_msgheader_t header;
    mbox_tagheader_t tag;
    
    union {
      // No request.
      struct {
        uint32_t base;
        uint32_t size;
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
  msg.tag.id = UINT32_C(0x00010006); // Get VC memory.
  msg.tag.size = sizeof(msg.value);
  msg.tag.code = 0;
  msg.footer.end = 0;

  memrange_t range;

  if (mbox_send(&msg) != 0) {
    range.base = range.size = 0;
  }
  else {
    range.base = msg.value.response.base;
    range.size = msg.value.response.size;
  }
  
  return range;
}

int prop_cmdline(char cmdline[static 256]) {
  typedef struct {
    mbox_msgheader_t header;
    mbox_tagheader_t tag;
    
    union {
      // No request.
      struct {
        uint8_t cmdline[256];
      }
      response;
    }
    value;

    mbox_msgfooter_t footer;
  }
  message_t;

  message_t msg __attribute__((aligned(16)));
  const uint32_t* source;
  uint32_t* dest;
  unsigned i;

  msg.header.size = sizeof(msg);
  msg.header.code = 0;
  msg.tag.id = UINT32_C(0x00050001); // Get command line.
  msg.tag.size = sizeof(msg.value);
  msg.tag.code = 0;
  msg.footer.end = 0;

  if (mbox_send(&msg) != 0) {
    return -1;
  }

  if ((msg.tag.code & 0x7fffffffU) < 255) {
    msg.value.response.cmdline[msg.tag.code & 0x7fffffffU] = 0;
  }
  else {
    msg.value.response.cmdline[255] = 0;
  }

  source = (uint32_t*)msg.value.response.cmdline;
  dest = (uint32_t*)cmdline;

  for (i = 0; i < 256; i += 4) {
    *dest++ = *source++;
  }

  return 0;
}

static uint32_t getclockrate(const uint32_t clock_id, const uint32_t tag_id) {
  typedef struct {
    mbox_msgheader_t header;
    mbox_tagheader_t tag;
    
    union {
      struct {
        uint32_t clock_id;
      }
      request;

      struct {
        uint32_t clock_id;
        uint32_t rate;
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
  msg.tag.id = tag_id;
  msg.tag.size = sizeof(msg.value);
  msg.tag.code = 0;
  msg.value.request.clock_id = clock_id;
  msg.footer.end = 0;

  if (mbox_send(&msg) != 0) {
    return 0;
  }

  return msg.value.response.rate;
}

uint32_t prop_getclockrate(const uint32_t clock_id) {
  return getclockrate(clock_id, UINT32_C(0x00030002));
}

uint32_t prop_getminclockrate(const uint32_t clock_id) {
  return getclockrate(clock_id, UINT32_C(0x00030007));
}

uint32_t prop_getmaxclockrate(const uint32_t clock_id) {
  return getclockrate(clock_id, UINT32_C(0x00030004));
}
