#include "armregs.h"
#include "memio.h"
#include "led.h"
#include "timer.h"

#include <stdint.h>

/*
This function is a regular C function with a  working function frame, so we
can write regular C code here without fear of touching the stack.
*/
static void __attribute__((noinline)) start(
  const uint32_t zero,
  const uint32_t machine_type,
  const uint32_t atags_addr) {

  // Zero the .bss section.
  extern uint32_t bss_start, bss_end;

  for (uint32_t* i = &bss_start; i < &bss_end; i++) {
    *i = zero;
  }

  memio_init();
  led_init();

  uint64_t t0 = timer();

  while (1) {
    led_set(1);
    timer_wait_until(t0 += 500000);
    led_set(0);
    timer_wait_until(t0 += 500000);
  }
}

/*
16 KB space to setup our initial stack.
*/
static uint64_t __attribute__((aligned(8))) stack0[2048];

/*
Only core 0 in a multicore CPU makes it here. The other cores are parked in a
busy loop waiting for an address to execute. We'll deal with them later. This
function is forced into section .start so that the linker script will put it
at 0x8000, where the bootloader expects it.

The parameters passed by the bootloader are documented at
https://github.com/torvalds/linux/blob/master/Documentation/arm/Booting

HACK! This function is naked, so we shouldn't write C code here. gcc generates
code that works just fine in this case because we only receive parameters via
register r0-r2 and don't create too much automatic variables, much being an
unknown quantity which depends on many things that we don't control. However,
lets *not* push it and keep this code as lean as possible. Also, always look
to kernel.lst and make sure the generated code is sane.
*/
void __attribute__((section(".kmain"), noreturn, naked)) kmain(
  const uint32_t zero,
  const uint32_t machine_type,
  const uint32_t atags_addr) {

  // Set the stack pointer to just below our kernel.
  set_sp((uint32_t)((uint8_t*)stack0 + sizeof(stack0)));

  // Call start with the CPU ID and the other parameters passed from the stub.
  start(zero, machine_type, atags_addr);

  // Do *not* return!
  while (1) {
    // Empty.
  }
}
