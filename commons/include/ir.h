#ifndef _IR_H__
#define _IR_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang.h"
#include "x86_64_codes.h"
#include "ir_opcodes.h"
#include "ir_funcs.h"
#include "ir_operands.h"
#include "ir_tables.h"
#include "ir_emit_funcs.h"
#include "ir_instr.h"

#define IR_BUFFER_INIT_CAPACITY 64

#define REX_PREFIX      0b0100
#define OPERANDS_64_BIT 1

//——————————————————————————————————————————————————————————————————————————————

lang_status_t ir_ctx_ctor(ir_ctx_t* ctx, size_t init_capacity);
lang_status_t ir_ctx_dtor(ir_ctx_t* ctx);

//——————————————————————————————————————————————————————————————————————————————

#endif // _IR_H__
