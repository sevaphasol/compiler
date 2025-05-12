//——————————————————————————————————————————————————————————————————————————————

#ifndef _IR_FUNCS_H__
#define _IR_FUNCS_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang_status.h"
#include "lang.h"
#include "ir_instr.h"
#include "bin_instr.h"

//——————————————————————————————————————————————————————————————————————————————

lang_status_t zeroary_opcode_ir_to_asm(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t unary_opcode_ir_to_asm(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t binary_opcode_ir_to_asm(lang_ctx_t* ctx, ir_instr_t* instr);

//——————————————————————————————————————————————————————————————————————————————

lang_status_t encode_nop          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_mul          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_div          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_push         (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_pop          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_call         (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_ret          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_syscall      (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_jmp          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_je           (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_jne          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_local_label  (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_global_label (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_two_operand_instr(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr);

//——————————————————————————————————————————————————————————————————————————————————————————————————

#endif // _IR_FUNCS_H__

//——————————————————————————————————————————————————————————————————————————————
