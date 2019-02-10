#include "gpio.h"
#include "prop.h"
#include "board.h"
#include "mbox.h"
#include "timer.h"

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
  const uint32_t revision = prop_revision();
  const board_t board = board_info(revision);

  if (board.model == BOARD_MODEL_A || board.model == BOARD_MODEL_B) {
    gpio_select(16, GPIO_OUTPUT);
    gpio_setpull(16, GPIO_PULL_OFF);
    led_set = set_16_inv;
  }
  else if (board.model == BOARD_MODEL_ZERO ||
           board.model == BOARD_MODEL_ZERO_W) {
    gpio_select(47, GPIO_OUTPUT);
    gpio_setpull(47, GPIO_PULL_OFF);
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
    gpio_setpull(29, GPIO_PULL_OFF);
    led_set = set_29;
  }
  else if (board.model == BOARD_MODEL_CM1 || board.model == BOARD_MODEL_CM3) {
    // These boards don't feature an activity LED.
    led_set = set_none;
  }
  else {
    gpio_select(47, GPIO_OUTPUT);
    gpio_setpull(47, GPIO_PULL_OFF);
    led_set = set_47;
  }

  return 0;
}

void led_blink(const unsigned number, const unsigned repeats)
{
  uint64_t time = timer();

  for (unsigned i = 0; i < repeats && repeats != 0; i++) {
    for (unsigned j = 0; j < number; j++) {
      led_set(1);
      timer_waituntil(time += 200000);
      led_set(0);
      timer_waituntil(time += 200000);
    }

    timer_waituntil(time += 300000);
  }
}
