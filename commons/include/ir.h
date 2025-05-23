#ifndef _IR_H__
#define _IR_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang.h"
#include "x86_64_codes.h"
#include "ir_opcodes.h"
#include "ir_to_asm_funcs.h"
#include "ir_operands.h"
#include "ir_tables.h"
#include "ir_emit_funcs.h"
#include "ir_instr.h"
#include "ir_list.h"

#define IR_BUFFER_INIT_CAPACITY 64

//——————————————————————————————————————————————————————————————————————————————

lang_status_t build_ir    (lang_ctx_t* ctx);
lang_status_t ir_to_binary(lang_ctx_t* ctx);
lang_status_t ir_to_asm   (lang_ctx_t* ctx);

//——————————————————————————————————————————————————————————————————————————————

#endif // _IR_H__
