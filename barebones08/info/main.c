#include "memio.h"
#include "arm.h"
#include "board.h"
#include "prop.h"

#include <stdio.h>
#include <inttypes.h>

int main(int argc, const char* const argv[]) {
  uint32_t const midr     = get_midr();
  uint32_t const variant  = (midr & MIDR_VARIANT_NO) >> 20;
  uint32_t const revision = midr & MIDR_REVISION;
  uint32_t const part_no  = midr & MIDR_PRIMARY_PART_NO;

  if (part_no == MIDR_PART_NO_ARM1176JZF_S) {
    printf("CPU:          ARM1176JZF-S r%" PRIu32 "p%" PRIu32 "\n", variant, revision);
  }
  else if (part_no == MIDR_PART_NO_CORTEX_A7) {
    printf("CPU:          Cortex-A7 r%" PRIu32 "p%" PRIu32 "\n", variant, revision);
  }
  else if (part_no == MIDR_PART_NO_CORTEX_A53) {
    printf("CPU:          Cortex-A53 r%" PRIu32 "p%" PRIu32 "\n", variant, revision);
  }

  board_t const board = board_info(prop_revision());
  printf(
    "Board:        %s %s %d.%d %u %s\n",
    board_model(board.model),
    board_processor(board.processor),
    board.rev_major,
    board.rev_minor,
    board.ram_mb,
    board_manufacturer(board.manufacturer)
  );

  printf("Base I/O:     0x%08" PRIx32 "\n", g_baseio);

  uint32_t   const fwrev   = prop_fwrev();
  uint32_t   const model   = prop_model();
  uint32_t   const rev     = prop_revision();
  uint64_t   const macaddr = prop_macaddr();
  uint64_t   const serial  = prop_serial();
  memrange_t const armmem  = prop_armmemory();
  memrange_t const vcmem   = prop_vcmemory();

  char cmdline[256];
  prop_cmdline(cmdline);

  printf("Firmware:     0x%08" PRIx32 "\n", fwrev);
  printf("Model:        0x%08" PRIx32 "\n", model);
  printf("Revision:     0x%08" PRIx32 "\n", rev);

  printf(
    "Mac addr:     %02" PRIx32 ":%02" PRIx32 ":%02" PRIx32
    ":%02" PRIx32 ":%02" PRIx32 ":%02" PRIx32 "\n",
    (uint32_t)((macaddr >> 40) & 255),
    (uint32_t)((macaddr >> 32) & 255),
    (uint32_t)((macaddr >> 24) & 255),
    (uint32_t)((macaddr >> 16) & 255),
    (uint32_t)((macaddr >> 8) & 255),
    (uint32_t)(macaddr & 255)
  );

  printf("Serial:       0x%016" PRIx64 "\n", serial);

  printf(
    "ARM memory:   base 0x%08" PRIx32 ", size 0x%08" PRIx32 "\n",
    armmem.base,
    armmem.size
  );

  printf(
    "VC memory:    base 0x%08" PRIx32 ", size 0x%08" PRIx32 "\n",
    vcmem.base,
    vcmem.size
  );

  static uint32_t const clock_id[] = {
    PROP_CLOCK_EMMC,  PROP_CLOCK_UART, PROP_CLOCK_ARM, PROP_CLOCK_CORE,
    PROP_CLOCK_V3D,   PROP_CLOCK_H264, PROP_CLOCK_ISP, PROP_CLOCK_SDRAM,
    PROP_CLOCK_PIXEL, PROP_CLOCK_PWM
  };

  static const char* const clock_name[] = {
    "emmc:",  "uart:", "arm:", "core:",
    "v3d:",   "h264:", "isp:", "sdram:",
    "pixel:", "pwm:"
  };

  for (unsigned i = 0; i < sizeof(clock_id) / sizeof(clock_id[0]); i++) {
    uint32_t const rate     = prop_getclockrate(clock_id[i]);
    uint32_t const min_rate = prop_getminclockrate(clock_id[i]);
    uint32_t const max_rate = prop_getmaxclockrate(clock_id[i]);

    printf(
      "Clock %-7s %" PRIu32 " Hz (%" PRIu32 ", %" PRIu32 ")\n",
      clock_name[i],
      rate,
      min_rate,
      max_rate
    );
  }

  return 0;
}
