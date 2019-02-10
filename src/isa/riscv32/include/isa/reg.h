#ifndef __RISCV32_REG_H__
#define __RISCV32_REG_H__

#include "common.h"
#include "memory.h"

#define PC_START (0x80000000u + IMAGE_START)

typedef struct {
  union {
    rtlreg_t _32;
    uint16_t _16[2];
    uint8_t _8[4];
  } gpr[32];

  vaddr_t pc;
  vaddr_t tvec;
  vaddr_t cause;
  vaddr_t epc;
  vaddr_t status;
  union {
    struct {
      uint32_t ppn :22;
      uint32_t asid: 9;
      uint32_t mode: 1;
    };
    uint32_t val;
  } satp;

  bool INTR;
} CPU_state;

static inline int check_reg_index(int index) {
  assert(index >= 0 && index < 32);
  return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)

static inline const char* reg_name(int index, int width) {
  extern const char* regsl[];
  assert(index >= 0 && index < 32);
  return regsl[index];
}

#endif