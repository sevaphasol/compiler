//——————————————————————————————————————————————————————————————————————————————

#ifndef _IR_FUNCS_H__
#define _IR_FUNCS_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang_status.h"
#include "lang.h"
#include "ir_instr.h"
#include "bin_instr.h"

//——————————————————————————————————————————————————————————————————————————————

lang_status_t asm_nop          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_add          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_sub          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_mul          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_div          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_mov          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_push         (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_pop          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_call         (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_ret          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_syscall      (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_jmp          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_je           (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_jne          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_test         (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_local_label  (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);
lang_status_t asm_global_label (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);

//——————————————————————————————————————————————————————————————————————————————————————————————————

lang_status_t encode_nop          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_add          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_sub          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_mul          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_div          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_mov          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_push         (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_pop          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_call         (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_ret          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_syscall      (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_jmp          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_je           (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_jne          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_test         (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_local_label  (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_global_label (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);

//——————————————————————————————————————————————————————————————————————————————————————————————————

#endif // _IR_FUNCS_H__

//——————————————————————————————————————————————————————————————————————————————
