#include "isr.h"
#include "glod.h"
#include "memio.h"

#include <stddef.h>

#define BASE_ADDR (g_baseio + UINT32_C(0x0000b000))

static void crash_reset(uint32_t const value) {
  (void)value;
  glod(GLOD_RESET);
}

static void crash_undefined(uint32_t const value) {
  (void)value;
  glod(GLOD_UNDEFINED);
}

static void crash_software(uint32_t const value) {
  (void)value;
  glod(GLOD_SOFTWARE);
}

static void crash_prefetch(uint32_t const value) {
  (void)value;
  glod(GLOD_PREFETCH);
}

static void crash_abort(uint32_t const value) {
  (void)value;
  glod(GLOD_ABORT);
}

static isr_handler_t s_handlers[] = {
  crash_reset, crash_undefined, crash_software, crash_prefetch,
  crash_abort, NULL, NULL
};

void isr_reshandler(void) {
  s_handlers[ISR_RESET](0);
}

void __attribute__((isr("UND"))) isr_undhandler(void) {
  uint32_t lr;
  __asm volatile("mov %[result], lr\n" : [result] "=r" (lr));

  s_handlers[ISR_UNDEFINED](lr - 4);
}

void __attribute__((isr("SWI"))) isr_swihandler(void) {
  s_handlers[ISR_SOFTWARE](0);
}

void __attribute__((isr("ABORT"))) isr_prehandler(void) {
  s_handlers[ISR_PREFETCH](0);
}

void __attribute__((isr("ABORT"))) isr_abthandler(void) {
  s_handlers[ISR_ABORT](0);
}

void __attribute__((isr("IRQ"))) isr_irqhandler(void) {
  s_handlers[ISR_IRQ](0);
}

void __attribute__((isr("FIQ"))) isr_fiqhandler(void) {
  s_handlers[ISR_FIQ](0);
}

isr_handler_t isr_sethandler(isr_t const isr, isr_handler_t const handler) {
  isr_handler_t const old = s_handlers[isr];
  s_handlers[isr] = handler;
  return old;
}

void isr_enablebasic(int const irq_number) {
  uint32_t const en1 = BASE_ADDR + 0x0210;
  uint32_t const en2 = BASE_ADDR + 0x0214;

  if (irq_number < 32) {
    mem_write32(en1, UINT32_C(1) << irq_number);
  }
  else {
    mem_write32(en2, UINT32_C(1) << (irq_number - 32));
  }
}

void isr_disablebasic(int const irq_number) {
  uint32_t const di1 = BASE_ADDR + 0x021c;
  uint32_t const di2 = BASE_ADDR + 0x0220;

  if (irq_number < 32) {
    mem_write32(di1, UINT32_C(1) << irq_number);
  }
  else {
    mem_write32(di2, UINT32_C(1) << (irq_number - 32));
  }
}
