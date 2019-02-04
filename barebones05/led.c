#include "gpio.h"
#include "prop.h"
#include "board.h"
#include "mbox.h"

static void set_16_inv(const int on) {
  gpio_set(16, !on);
}

static void set_47_inv(const int on) {
  gpio_set(47, !on);
}

static void set_130(const int on) {
  typedef struct {
    mbox_msgheader_t header;
    mbox_tagheader_t tag;
    
    union {
      struct {
        uint32_t pin;
        uint32_t value;
      }
      request;
      // No response.
    }
    value;

    mbox_msgfooter_t footer;
  }
  message_t;

  message_t msg __attribute__((aligned(16)));

  msg.header.size = sizeof(msg);
  msg.header.code = 0;
  msg.tag.id = UINT32_C(0x00038041);
  msg.tag.size = sizeof(msg.value);
  msg.tag.code = 0;
  msg.value.request.pin = 130;
  msg.value.request.value = !!on;
  msg.footer.end = 0;

  mbox_send(&msg);
}

static void set_29(const int on) {
  gpio_set(29, on);
}

static void set_none(const int on) {
  (void)on;
}

static void set_47(const int on) {
  gpio_set(47, on);
}

void (*led_set)(const int on) = set_none;

int led_init(void) {
  board_t board;
  const uint32_t revision = prop_get_revision();

  {
    int res;
    
    if ((res = board_info(&board, revision)) != 0) {
      return res;
    }
  }

  if (board.model == BOARD_MODEL_A || board.model == BOARD_MODEL_B) {
    gpio_select(16, GPIO_OUTPUT);
    led_set = set_16_inv;
  }
  else if (board.model == BOARD_MODEL_ZERO ||
           board.model == BOARD_MODEL_ZERO_W) {
    gpio_select(47, GPIO_OUTPUT);
    led_set = set_47_inv;
  }
  else if (board.model == BOARD_MODEL_3B &&
           board.rev_major == 1 &&
           board.rev_minor == 2) {
    // GPIO expander, output only.
    led_set = set_130;
  }
  else if (board.model == BOARD_MODEL_3A_PLUS ||
           board.model == BOARD_MODEL_3B_PLUS) {
    gpio_select(29, GPIO_OUTPUT);
    led_set = set_29;
  }
  else if (board.model == BOARD_MODEL_CM1 || board.model == BOARD_MODEL_CM3) {
    // These boards don't feature an activity LED.
    led_set = set_none;
  }
  else {
    gpio_select(47, GPIO_OUTPUT);
    led_set = set_47;
  }

  return 0;
}
