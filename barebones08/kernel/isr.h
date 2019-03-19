#ifndef IRQ_H__
#define IRQ_H__

#include <stdint.h>

typedef enum {
  ISR_RESET     = 0,
  ISR_UNDEFINED = 1,
  ISR_SOFTWARE  = 2,
  ISR_PREFETCH  = 3,
  ISR_ABORT     = 4,
  ISR_IRQ       = 5,
  ISR_FIQ       = 6
}
isr_t;

typedef void (*isr_handler_t)(uint32_t const value);

isr_handler_t isr_sethandler(isr_t const isr, isr_handler_t const handler);
void isr_enablebasic(int const irq_number);
void isr_disablebasic(int const irq_number);

#endif /* IRQ_H__ */
