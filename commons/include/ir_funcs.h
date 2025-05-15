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
lang_status_t idiv_ir_to_asm(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t lib_func_ir_to_asm  (lang_ctx_t*  ctx, ir_instr_t*  ir_instr);

//——————————————————————————————————————————————————————————————————————————————

lang_status_t encode_nop          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_add          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_sub          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_imul         (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_idiv         (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
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
lang_status_t encode_in           (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_out          (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_local_label  (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);
lang_status_t encode_global_label (lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr);

//——————————————————————————————————————————————————————————————————————————————————————————————————

#endif // _IR_FUNCS_H__

//——————————————————————————————————————————————————————————————————————————————
