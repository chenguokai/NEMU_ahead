/***************************************************************************************
* Copyright (c) 2020-2022 Institute of Computing Technology, Chinese Academy of Sciences
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <common.h>
#ifdef CONFIG_RVV_010

#include "cpu/exec.h"
#include "../local-include/vreg.h"
#include "../local-include/csr.h"
#include <stdio.h>
#include "../local-include/intr.h"
#include "../local-include/rtl.h"
#include <setjmp.h>

#define id_src (&s->src1)
#define id_src2 (&s->src2)
#define id_dest (&s->dest)
void vp_set_dirty();

def_EHelper(vsetvl) {

  vp_set_dirty();
  //vlmul+lg2(VLEN) <= vsew + vl
  // previous decode does not load vals for us
  rtl_lr(s, &(id_src->val), id_src1->reg, 4);
  rtlreg_t vl = check_vsetvl(id_src2->val, id_src->val, id_src->reg==0);
  rtlreg_t error = 1ul << 63;
  if(vl==(uint64_t)-1) vcsr_write(IDXVTYPE, &error); //TODO: may cause error.
  else vcsr_write(IDXVTYPE, &(id_src2->val));
  vcsr_write(IDXVL, &vl);

  rtl_sr(s, id_dest->reg, &vl, 8/*4*/);

  rtl_li(s, &(s->tmp_reg[0]), 0);
  vcsr_write(IDXVSTART, &(s->tmp_reg[0]));

  // print_asm_template3(vsetvl);
}

#endif // CONFIG_RVV_010