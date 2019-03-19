#ifndef PROP_H__
#define PROP_H__

#include <stdint.h>

#define PROP_CLOCK_EMMC   1
#define PROP_CLOCK_UART   2
#define PROP_CLOCK_ARM    3
#define PROP_CLOCK_CORE   4
#define PROP_CLOCK_V3D    5
#define PROP_CLOCK_H264   6
#define PROP_CLOCK_ISP    7
#define PROP_CLOCK_SDRAM  8
#define PROP_CLOCK_PIXEL  9
#define PROP_CLOCK_PWM   10

typedef struct {
  uint32_t base;
  uint32_t size;
}
memrange_t;

uint32_t   prop_revision(void);
uint32_t   prop_fwrev(void);
uint32_t   prop_model(void);
uint64_t   prop_macaddr(void);
uint64_t   prop_serial(void);
memrange_t prop_armmemory(void);
memrange_t prop_vcmemory(void);
int        prop_cmdline(char cmdline[static 256]);
uint32_t   prop_getclockrate(uint32_t const clock_id);
uint32_t   prop_getminclockrate(uint32_t const clock_id);
uint32_t   prop_getmaxclockrate(uint32_t const clock_id);

#endif /* PROP_H__ */
