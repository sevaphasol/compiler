//——————————————————————————————————————————————————————————————————————————————

#ifndef _IR_EMIT_FUNCS_H__
#define _IR_EMIT_FUNCS_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang.h"
#include "ir_opcodes.h"
#include "ir_operands.h"

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_nop              (lang_ctx_t* ctx);
lang_status_t emit_add              (lang_ctx_t* ctx, ir_opd_t opd1, ir_opd_t opd2);
lang_status_t emit_sub              (lang_ctx_t* ctx, ir_opd_t opd1, ir_opd_t opd2);
lang_status_t emit_mul              (lang_ctx_t* ctx, ir_opd_t opd1, ir_opd_t opd2);
lang_status_t emit_div              (lang_ctx_t* ctx, ir_opd_t opd1, ir_opd_t opd2);
lang_status_t emit_mov              (lang_ctx_t* ctx, ir_opd_t src, ir_opd_t dst);
lang_status_t emit_push             (lang_ctx_t* ctx, ir_opd_t opd);
lang_status_t emit_pop              (lang_ctx_t* ctx, ir_opd_t opd);
lang_status_t emit_call             (lang_ctx_t* ctx, const char* name);
lang_status_t emit_ret              (lang_ctx_t* ctx);
lang_status_t emit_global_label     (lang_ctx_t* ctx, const char* name);
lang_status_t emit_local_label      (lang_ctx_t* ctx, int64_t label_number);
lang_status_t emit_jmp              (lang_ctx_t* ctx, int64_t label_number);
lang_status_t emit_je               (lang_ctx_t* ctx, int64_t label_number);
lang_status_t emit_jne              (lang_ctx_t* ctx, int64_t label_number);
lang_status_t emit_syscall          (lang_ctx_t* ctx);
lang_status_t emit_test             (lang_ctx_t* ctx, ir_opd_t opd1, ir_opd_t opd2);
lang_status_t emit_binary_operation (lang_ctx_t* ctx, ir_opc_t opcode, ir_opd_t opd1, ir_opd_t opd2);
lang_status_t ir_emit_operation     (ir_ctx_t* ctx, ir_instr_t instr);

//——————————————————————————————————————————————————————————————————————————————

#endif // _IR_EMIT_FUNCS_H__

//——————————————————————————————————————————————————————————————————————————————
