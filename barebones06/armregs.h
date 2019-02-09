#ifndef ARMREGS_H__
#define ARMREGS_H__

#include <stdint.h>

/****************************************************************************\
| Stack Pointer                                                              |
\****************************************************************************/

inline uint32_t get_sp(void) {
  uint32_t x;
  __asm volatile("mov %[result], sp\n" : [result] "=r" (x));
  return x;
}

inline void set_sp(const uint32_t x) {
  __asm volatile("mov sp, %[value]\n" : : [value] "r" (x) : "sp");
}

/****************************************************************************\
| Main ID Register                                                           |
\****************************************************************************/

#define MIDR_REVISION        (UINT32_C(0xf)   <<  0)
#define MIDR_PRIMARY_PART_NO (UINT32_C(0xfff) <<  4)
#define MIDR_ARCHITECTURE    (UINT32_C(0xf)   << 16)
#define MIDR_VARIANT_NO      (UINT32_C(0xf)   << 20)
#define MIDR_IMPLEMENTOR     (UINT32_C(0xff)  << 24)

#define MIDR_PART_NO_ARM1176JZF_S 0xb760
#define MIDR_PART_NO_CORTEX_A7    0xc070
#define MIDR_PART_NO_CORTEX_A53   0xd030

inline uint32_t get_midr(void) {
  uint32_t x;
  __asm volatile("mrc p15, 0, %[result], c0, c0, #0\n" : [result] "=r" (x));
  return x;
}

/****************************************************************************\
| Current Program Status Register                                            |
\****************************************************************************/

#define CPSR_M  (UINT32_C(0x1f) <<  0)
#define CPSR_T  (UINT32_C(1)    <<  5)
#define CPSR_F  (UINT32_C(1)    <<  6)
#define CPSR_I  (UINT32_C(1)    <<  7)
#define CPSR_A  (UINT32_C(1)    <<  8)
#define CPSR_E  (UINT32_C(1)    <<  9)
#define CPSR_GE (UINT32_C(0xf)  << 16)
#define CPSR_J  (UINT32_C(1)    << 24)
#define CPSR_Q  (UINT32_C(1)    << 27)
#define CPSR_V  (UINT32_C(1)    << 28)
#define CPSR_C  (UINT32_C(1)    << 29)
#define CPSR_Z  (UINT32_C(1)    << 30)
#define CPSR_N  (UINT32_C(1)    << 31)

#define CPSR_M_USR UINT32_C(0x10)
#define CPSR_M_FIQ UINT32_C(0x11)
#define CPSR_M_IRQ UINT32_C(0x12)
#define CPSR_M_SVC UINT32_C(0x13)
#define CPSR_M_MON UINT32_C(0x16)
#define CPSR_M_ABT UINT32_C(0x17)
#define CPSR_M_HYP UINT32_C(0x1a)
#define CPSR_M_UND UINT32_C(0x1b)
#define CPSR_M_SYS UINT32_C(0x1f)

inline uint32_t get_cpsr(void) {
  uint32_t x;
  __asm volatile("mrs %[result], cpsr\n" : [result] "=r" (x));
  return x;
}

inline void set_cpsr(const uint32_t x) {
  __asm volatile("msr cpsr_cxsf, %[value]\n" : : [value] "r" (x) : "cc");
}

/****************************************************************************\
| Saved Program Status Register                                              |
\****************************************************************************/

inline void set_spsr(const uint32_t x) {
  __asm volatile("msr spsr_cxsf, %[value]\n" : : [value] "r" (x));
}

/****************************************************************************\
| Non-Secure Access Control Register                                         |
\****************************************************************************/

#define NSACR_CP11     (UINT32_C(1) << 10)
#define NSACR_CP10     (UINT32_C(1) << 11)
#define NSACR_NSD32DIS (UINT32_C(1) << 14)
#define NSACR_NSASEDIS (UINT32_C(1) << 15)
#define NSACR_NS_L2ERR (UINT32_C(1) << 17)
#define NSACR_NS_SMP   (UINT32_C(1) << 18)

inline uint32_t get_nsacr(void) {
  uint32_t x;
  __asm volatile("mrc p15, 0, %[result], c1, c1, #2\n" : [result] "=r" (x));
  return x;
}

inline void set_nsacr(const uint32_t x) {
  __asm volatile("mcr p15, 0, %[value], c1, c1, #2\n" : : [value] "r" (x));
}

/****************************************************************************\
| Coprocessor Access Control Register                                        |
\****************************************************************************/

#define CPACR_CP0  (UINT32_C(0x3) <<  0)
#define CPACR_CP1  (UINT32_C(0x3) <<  2)
#define CPACR_CP2  (UINT32_C(0x3) <<  4)
#define CPACR_CP3  (UINT32_C(0x3) <<  6)
#define CPACR_CP4  (UINT32_C(0x3) <<  8)
#define CPACR_CP5  (UINT32_C(0x3) << 10)
#define CPACR_CP6  (UINT32_C(0x3) << 12)
#define CPACR_CP7  (UINT32_C(0x3) << 14)
#define CPACR_CP8  (UINT32_C(0x3) << 16)
#define CPACR_CP9  (UINT32_C(0x3) << 18)
#define CPACR_CP10 (UINT32_C(0x3) << 20)
#define CPACR_CP11 (UINT32_C(0x3) << 22)
#define CPACR_CP12 (UINT32_C(0x3) << 24)
#define CPACR_CP13 (UINT32_C(0x3) << 26)

inline uint32_t get_cpacr(void) {
  uint32_t x;
  __asm volatile("mrc p15, 0, %[result], c1, c0, #2\n" : [result] "=r" (x));
  return x;
}

inline void set_cpacr(const uint32_t x) {
  __asm volatile("mcr p15, 0, %[value], c1, c0, #2\n" : : [value] "r" (x));
}

/****************************************************************************\
| System Control Register                                                    |
\****************************************************************************/

#define SCTLR_M  (UINT32_C(1) <<  0)
#define SCTLR_A  (UINT32_C(1) <<  1)
#define SCTLR_C  (UINT32_C(1) <<  2)
#define SCTLR_W  (UINT32_C(1) <<  3)
#define SCTLR_B  (UINT32_C(1) <<  7)
#define SCTLR_Z  (UINT32_C(1) << 11)
#define SCTLR_I  (UINT32_C(1) << 12)
#define SCTLR_V  (UINT32_C(1) << 13)
#define SCTLR_RR (UINT32_C(1) << 14)
#define SCTLR_L4 (UINT32_C(1) << 15)
#define SCTLR_FI (UINT32_C(1) << 21)
#define SCTLR_U  (UINT32_C(1) << 22)
#define SCTLR_XP (UINT32_C(1) << 23)
#define SCTLR_VE (UINT32_C(1) << 24)
#define SCTLR_EE (UINT32_C(1) << 25)
#define SCTLR_TR (UINT32_C(1) << 28)
#define SCTLR_FA (UINT32_C(1) << 29)

inline uint32_t get_sctlr(void) {
  uint32_t x;
  __asm volatile("mrc p15, 0, %[result], c1, c0, #0\n" : [result] "=r" (x));
  return x;
}

inline void set_sctlr(const uint32_t x) {
  __asm volatile("mcr p15, 0, %[value], c1, c0, #0\n" : : [value] "r" (x));
}

/****************************************************************************\
| Floating-Point Exception Register                                          |
\****************************************************************************/

#define FPEXC_IOC    (UINT32_C(1)   <<  0)
#define FPEXC_OFC    (UINT32_C(1)   <<  2)
#define FPEXC_UFC    (UINT32_C(1)   <<  3)
#define FPEXC_INV    (UINT32_C(1)   <<  7)
#define FPEXC_VECITR (UINT32_C(0x7) <<  8)
#define FPEXC_FP2V   (UINT32_C(1)   << 28)
#define FPEXC_EN     (UINT32_C(1)   << 30)
#define FPEXC_EX     (UINT32_C(1)   << 31)

inline void set_fpexc(const uint32_t x) {
  __asm volatile("vmsr fpexc, %[value]\n" : : [value] "r" (x));
}

#endif /* ARMREGS_H__ */
