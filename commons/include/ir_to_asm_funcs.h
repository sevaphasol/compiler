#ifndef _IR_TO_ASM_FUNCS_H__
#define _IR_TO_ASM_FUNCS_H__

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

#endif // _IR_TO_ASM_FUNCS_H__
