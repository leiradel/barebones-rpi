#include "gpio.h"
#include "prop.h"
#include "board.h"
#include "mbox.h"
#include "timer.h"

void (*led_set)(int const on) = 0;

static void set_16_inv(int const on) {
  gpio_set(16, !on);
}

static void set_47_inv(int const on) {
  gpio_set(47, !on);
}

static void set_130(int const on) {
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

  message_t msg __attribute__((aligned(16))) = {
    .header.size = sizeof(msg),
    .header.code = 0,
    .tag.id = UINT32_C(0x00038041),
    .tag.size = sizeof(msg.value),
    .tag.code = 0,
    .value.request.pin = 130,
    .value.request.value = !!on,
    .footer.end = 0
  };

  mbox_send(&msg);
}

static void set_29(int const on) {
  gpio_set(29, on);
}

static void set_none(int const on) {
  (void)on;
}

static void set_47(int const on) {
  gpio_set(47, on);
}

int led_init(void) {
  uint32_t const revision = prop_revision();
  board_t  const board    = board_info(revision);

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

void led_blink(unsigned const number, unsigned const repeats)
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
