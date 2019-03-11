#include "armregs.h"
#include "memio.h"
#include "led.h"
#include "glod.h"

#include <stdint.h>

// 16 KB space to setup our initial stack.
static uint64_t svc_stack[2048] __attribute__((aligned(8)));

// 256 bytes stacks for fatal exceptions.
static uint64_t glod_stack[32] __attribute__((aligned(8)));

// 256 bytes stacks for IRQ.
static uint64_t irq_stack[32] __attribute__((aligned(8)));

// 256 bytes stacks for FIQ.
static uint64_t fiq_stack[32] __attribute__((aligned(8)));

// Zero the .bss section.
static void zero_bss(const uint32_t zero) {
  extern uint32_t bss_start, bss_end;

  for (uint32_t* i = &bss_start; i < &bss_end; i++) {
    *i = zero;
  }
}

/*
This code will put the core into the mode specified in spsr. GNU gas doesn't
support `msr ELR_hyp, r0` and `eret` in ARMv6 and ARMv7, so we use hexadecimal
constants to emit the instructions.
*/
static inline void exit_hyp(void) {
  __asm volatile(
    "add r0, pc, #4\n"
    ".long 0xe12ef300 @ msr	elr_hyp, r0\n"
    ".long 0xe160006e @ eret\n"
    :
    :
    : "r0", "cc"
  );
}

// Put the core in supervisor mode.
static void enter_svc_mode(void) {
  const uint32_t cpsr = get_cpsr();
  const uint32_t current_mode = cpsr & CPSR_M;
  const uint32_t svc_mode = (cpsr & ~CPSR_M) | CPSR_M_SVC;
  
  if (current_mode == CPSR_M_HYP) {
    // Save the banked hypervisor registers and set them back after exiting
    // this mode. Only the stack pointer is banked in hypervisor.
    const uint32_t sp = get_sp();

    // Set the SPSR value to enter supervisor mode.
    set_spsr(svc_mode);

    // Exit hypervisor mode to the mode in `svc_mode`.
    exit_hyp();

    // Set the value of the banked registers.
    set_sp(sp);
  }
  else if (current_mode != CPSR_M_SVC) {
    // For other modes it's enough to set CPSR.
    set_cpsr(svc_mode);
  }
}

/*
Turn on L1 data and instruction caches, and branch prediction flags in the
Control Register.
*/
static void enable_l1_and_branch(void) {
  const uint32_t sctlr = get_sctlr();
  set_sctlr(sctlr | SCTLR_C | SCTLR_I | SCTLR_Z);
}

/*
Enable CP10 and CP11 in both secure and non-secore modes, and in both
priviedged and unpriviledged modes.
*/
static void enable_vfp(void) {
  const uint32_t nsacr = get_nsacr();
  const uint32_t cp10_and_cp11_enabled = NSACR_CP10 | NSACR_CP11;

  // Enable CP10 and CP11 in both secure and non-secore modes, if necessary.
  if ((nsacr & cp10_and_cp11_enabled) != cp10_and_cp11_enabled) {
    set_nsacr(nsacr | cp10_and_cp11_enabled);
  }

  // Enable CP10 and CP11 in both priviledged and non-priviledged modes.
  const uint32_t cpacr = get_cpacr();
  set_cpacr(cpacr | CPACR_CP10 | CPACR_CP11);

  // Enable VFP.
  set_fpexc(FPEXC_EN);
}

/*
Set up the stacks for all exception modes. Careful here as we're dealing with
the stack pointer.
*/
static void setup_stacks(void) {
  // Set up the exception stacks.
  const uint32_t cpsr = get_cpsr();

  const uint32_t und_mode = (cpsr & ~CPSR_M) | CPSR_M_UND;
  set_cpsr(und_mode);
  set_sp((uint32_t)((uint8_t*)glod_stack + sizeof(glod_stack)));

  const uint32_t abt_mode = (cpsr & ~CPSR_M) | CPSR_M_ABT;
  set_cpsr(abt_mode);
  set_sp((uint32_t)((uint8_t*)glod_stack + sizeof(glod_stack)));

  const uint32_t irq_mode = (cpsr & ~CPSR_M) | CPSR_M_IRQ;
  set_cpsr(irq_mode);
  set_sp((uint32_t)((uint8_t*)irq_stack + sizeof(irq_stack)));

  const uint32_t fiq_mode = (cpsr & ~CPSR_M) | CPSR_M_FIQ;
  set_cpsr(fiq_mode);
  set_sp((uint32_t)((uint8_t*)fiq_stack + sizeof(fiq_stack)));

  // Go back to the previous mode.
  set_cpsr(cpsr);
}

// Set up the exception vector.
static void setup_isr_table(void) {
  uint32_t* isr;

  // Cheat the compiler to get a pointer to 0. Writing isr = NULL upsets the
  // compiler, and it generates completely buggy code.
  __asm volatile("mov %[pointer], #0\n" : [pointer] "=r" (isr));

  for (int i = 0; i < 8; i++) {
    *isr++ = UINT32_C(0xe59ff018); // ldr pc, [pc, #24]
  }

  extern void res_handler(void);
  extern void und_handler(void);
  extern void swi_handler(void);
  extern void pre_handler(void);
  extern void abt_handler(void);
  extern void rsr_handler(void);
  extern void irq_handler(void);
  extern void fiq_handler(void);

  *isr++ = (uint32_t)res_handler;
  *isr++ = (uint32_t)und_handler;
  *isr++ = (uint32_t)swi_handler;
  *isr++ = (uint32_t)pre_handler;
  *isr++ = (uint32_t)abt_handler;
  *isr++ = (uint32_t)rsr_handler; // unused
  *isr++ = (uint32_t)irq_handler;
  *isr   = (uint32_t)fiq_handler;
}

/*
This function is a regular C function with a  working function frame, so we
can write regular C code here without fear of touching the stack.
*/
static void __attribute__((noinline)) start(
  const uint32_t zero,
  const uint32_t machine_type,
  const uint32_t atags_addr) {

  zero_bss(zero);
  enter_svc_mode();
  enable_l1_and_branch();
  enable_vfp();
  setup_isr_table();
  setup_stacks();

  memio_init();
  led_init();

  extern void main(const uint32_t, const uint32_t);
  main(machine_type, atags_addr);

  // Do *not* return!
  glod(GLOD_EXITED);
}

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
void __attribute__((section(".kmain"), naked)) kmain(
  const uint32_t zero,
  const uint32_t machine_type,
  const uint32_t atags_addr) {

  // Set the stack pointer to just below our kernel.
  set_sp((uint32_t)((uint8_t*)svc_stack + sizeof(svc_stack)));

  // Call start with the CPU ID and the other parameters passed from the stub.
  start(zero, machine_type, atags_addr);

  // Do *not* return! No, really!
  while (1) {
    // Empty.
  }
}
