#include "board.h"

#include <stddef.h>

// https://www.raspberrypi.org/documentation/hardware/raspberrypi/revision-codes/README.md
int board_info(board_t* board, const uint32_t revision) {
  static const board_t boards[] = {
    /* 0002 */ {BOARD_MODEL_B, BOARD_PROCESSOR_BCM2835,
                1, 0, 256, BOARD_MANUFACTURER_EGOMAN},
    /* 0003 */ {BOARD_MODEL_B, BOARD_PROCESSOR_BCM2835,
                1, 0, 256, BOARD_MANUFACTURER_EGOMAN},
    /* 0004 */ {BOARD_MODEL_B, BOARD_PROCESSOR_BCM2835,
                2, 0, 256, BOARD_MANUFACTURER_SONY_UK},
    /* 0005 */ {BOARD_MODEL_B, BOARD_PROCESSOR_BCM2835,
                2, 0, 256, BOARD_MANUFACTURER_QISDA},
    /* 0006 */ {BOARD_MODEL_B, BOARD_PROCESSOR_BCM2835,
                2, 0, 256, BOARD_MANUFACTURER_EGOMAN},
    /* 0007 */ {BOARD_MODEL_A, BOARD_PROCESSOR_BCM2835,
                2, 0, 256, BOARD_MANUFACTURER_EGOMAN},
    /* 0008 */ {BOARD_MODEL_A, BOARD_PROCESSOR_BCM2835,
                2, 0, 256, BOARD_MANUFACTURER_SONY_UK},
    /* 0009 */ {BOARD_MODEL_A, BOARD_PROCESSOR_BCM2835,
                2, 0, 256, BOARD_MANUFACTURER_QISDA},
    /*      */ {0},
    /*      */ {0},
    /*      */ {0},
    /* 000d */ {BOARD_MODEL_B, BOARD_PROCESSOR_BCM2835,
                2, 0, 512, BOARD_MANUFACTURER_EGOMAN},
    /* 000e */ {BOARD_MODEL_B, BOARD_PROCESSOR_BCM2835,
                2, 0, 512, BOARD_MANUFACTURER_SONY_UK},
    /* 000f */ {BOARD_MODEL_B, BOARD_PROCESSOR_BCM2835,
                2, 0, 512, BOARD_MANUFACTURER_EGOMAN},
    /* 0010 */ {BOARD_MODEL_B_PLUS, BOARD_PROCESSOR_BCM2835,
                1, 0, 512, BOARD_MANUFACTURER_SONY_UK},
    /* 0011 */ {BOARD_MODEL_CM1, BOARD_PROCESSOR_BCM2835,
                1, 0, 512, BOARD_MANUFACTURER_SONY_UK},
    /* 0012 */ {BOARD_MODEL_A_PLUS, BOARD_PROCESSOR_BCM2835,
                1, 1, 256, BOARD_MANUFACTURER_SONY_UK},
    /* 0013 */ {BOARD_MODEL_B_PLUS, BOARD_PROCESSOR_BCM2835,
                1, 2, 512, BOARD_MANUFACTURER_EMBEST},
    /* 0014 */ {BOARD_MODEL_CM1, BOARD_PROCESSOR_BCM2835,
                1, 0, 512, BOARD_MANUFACTURER_EMBEST},
    /* 0015 */ {BOARD_MODEL_A_PLUS, BOARD_PROCESSOR_BCM2835,
                1, 1,   0, BOARD_MANUFACTURER_EMBEST},
  };

  if ((revision & UINT32_C(0x00800000)) == 0) {
    // Old style revision, lookup in the board table.
      *board = boards[revision - 2];
  }
  else {
    // New style revision, extract the fields.
    board->model = (revision >> 4) & 0xff;
    board->processor = (revision >> 12) & 0x0f;
    board->rev_major = 1;
    board->rev_minor = revision & 0x0f;
    board->ram_mb = 256 << ((revision >> 20) & 0x07);
    board->manufacturer = (revision >> 16) & 0x0f;

    if (board->manufacturer == 4) {
      board->manufacturer = BOARD_MANUFACTURER_EMBEST;
    }
  }

  return 0;
}
