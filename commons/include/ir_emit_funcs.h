#ifndef _IR_EMIT_FUNCS_H__
#define _IR_EMIT_FUNCS_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang.h"
#include "ir_opcodes.h"
#include "ir_operands.h"

//——————————————————————————————————————————————————————————————————————————————

lang_status_t ir_emit_instr(buffer_t* ir_buf, ir_instr_t ir_instr);

//——————————————————————————————————————————————————————————————————————————————

#endif // _IR_EMIT_FUNCS_H__
