#include "glod.h"

void res_handler(void) {
  glod(GLOD_RESET);
}

void __attribute__((isr("UND"))) und_handler(void) {
  glod(GLOD_UNDEFINED);
}

void __attribute__((isr("ABORT"))) pre_handler(void) {
  glod(GLOD_PREFETCH);
}

void __attribute__((isr("ABORT"))) abt_handler(void) {
  glod(GLOD_ABORT);
}

void rsr_handler(void) {
  glod(GLOD_RESERVED);
}

void __attribute__((isr("SWI"))) swi_handler(void) {}
void __attribute__((isr("IRQ"))) irq_handler(void) {}
void __attribute__((isr("FIQ"))) fiq_handler(void) {}
