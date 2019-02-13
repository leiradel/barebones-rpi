#include "armregs.h"
#include "aux.h"
#include "board.h"
#include "memio.h"
#include "prop.h"

#include <stdio.h>
#include <inttypes.h>

void __attribute__((section(".main"))) main(
  const uint32_t machine_type,
  const uint32_t atags_addr) {

  uart_init(115200);

  const uint32_t midr = get_midr();
  const uint32_t variant = (midr & MIDR_VARIANT_NO) >> 20;
  const uint32_t revision = midr & MIDR_REVISION;
  const uint32_t part_no = midr & MIDR_PRIMARY_PART_NO;

  if (part_no == MIDR_PART_NO_ARM1176JZF_S) {
    printf("CPU:          ARM1176JZF-S r%up%u\n", variant, revision);
  }
  else if (part_no == MIDR_PART_NO_CORTEX_A7) {
    printf("CPU:          Cortex-A7 r%up%u\n", variant, revision);
  }
  else if (part_no == MIDR_PART_NO_CORTEX_A53) {
    printf("CPU:          Cortex-A53 r%up%u\n", variant, revision);
  }

  const board_t board = board_info(prop_revision());
  printf(
    "Board:        %s %s %u.%u %u %s\n",
    board_model(board.model),
    board_processor(board.processor),
    board.rev_major,
    board.rev_minor,
    board.ram_mb,
    board_manufacturer(board.manufacturer)
  );

  printf("Machine type: 0x%08" PRIx32 "\n", machine_type);
  printf("Tags:         0x%08" PRIx32 "\n", atags_addr);
  printf("Base I/O:     0x%08" PRIx32 "\n", g_baseio);

  const uint32_t fwrev = prop_fwrev();
  const uint32_t model = prop_model();
  const uint32_t rev = prop_revision();
  const uint64_t macaddr = prop_macaddr();
  const uint64_t serial = prop_serial();
  const memrange_t armmem = prop_armmemory();
  const memrange_t vcmem = prop_vcmemory();

  char cmdline[256];
  prop_cmdline(cmdline);

  printf("Firmware:     0x%08" PRIx32 "\n", fwrev);
  printf("Model:        0x%08" PRIx32 "\n", model);
  printf("Revision:     0x%08" PRIx32 "\n", rev);

  printf(
    "Mac addr:     %02x:%02x:%02x:%02x:%02x:%02x\n",
    (macaddr >> 40) & 255,
    (macaddr >> 32) & 255,
    (macaddr >> 24) & 255,
    (macaddr >> 16) & 255,
    (macaddr >> 8) & 255,
    macaddr & 255
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

  static const uint32_t clock_id[] = {
    PROP_CLOCK_EMMC, PROP_CLOCK_UART, PROP_CLOCK_ARM, PROP_CLOCK_CORE,
    PROP_CLOCK_V3D, PROP_CLOCK_H264, PROP_CLOCK_ISP, PROP_CLOCK_SDRAM,
    PROP_CLOCK_PIXEL, PROP_CLOCK_PWM
  };

  static const char* const clock_name[] = {
    "emmc:", "uart:", "arm:", "core:", "v3d:",
    "h264:", "isp:", "sdram:", "pixel:", "pwm:"
  };

  for (unsigned i = 0; i < sizeof(clock_id) / sizeof(clock_id[0]); i++) {
    const uint32_t rate = prop_getclockrate(clock_id[i]);
    const uint32_t min_rate = prop_getminclockrate(clock_id[i]);
    const uint32_t max_rate = prop_getmaxclockrate(clock_id[i]);

    printf(
      "Clock %-7s %u Hz (%u, %u)\n",
      clock_name[i],
      rate,
      min_rate,
      max_rate
    );
  }
}
