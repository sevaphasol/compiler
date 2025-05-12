//——————————————————————————————————————————————————————————————————————————————

#ifndef _IR_DSL_H__
#define _IR_DSL_H__

#include "ir.h"

//——————————————————————————————————————————————————————————————————————————————

/* IR operands */

#define OPD_REG(_reg) {.type = IR_OPD_REGISTER,  .value = {.reg    = _reg}}
#define OPD_MEM(_off) {.type = IR_OPD_MEMORY,    .value = {.offset = _off}}
#define OPD_IMM(_imm) {.type = IR_OPD_IMMEDIATE, .value = {.imm    = _imm}}

#define OPD_GLOBAL_LABEL(_name) \
    {.type = IR_OPD_GLOBAL_LABEL, .value = {.global_label_name  = _name}}
#define OPD_LOCAL_LABEL(_num) \
    {.type = IR_OPD_LOCAL_LABEL,  .value = {.local_label_number = _num}}

//——————————————————————————————————————————————————————————————————————————————

/* IR operations */

#define OP_ADD(_opd1, _opd2)  {IR_OPC_ADD,  _opd1, _opd2}
#define OP_SUB(_opd1, _opd2)  {IR_OPC_SUB,  _opd1, _opd2}
#define OP_MUL(_opd1, _opd2)  {IR_OPC_MUL,  _opd1, _opd2}
#define OP_DIV(_opd1, _opd2)  {IR_OPC_DIV,  _opd1, _opd2}
#define OP_MOV(_opd1, _opd2)  {IR_OPC_MOV,  _opd1, _opd2}
#define OP_TEST(_opd1, _opd2) {IR_OPD_TEST, _opd1, _opd2}

#define OP_PUSH(_opd) {IR_OPC_PUSH, _opd, {}}
#define OP_POP(_opd)  {IR_OPC_POP,  _opd, {}}
#define OP_CALL(_opd) {IR_OPC_CALL, _opd, {}}
#define OP_JMP(_opd)  {IR_OPD_JMP,  _opd, {}}
#define OP_JE(_opd)   {IR_OPD_JE,   _opd, {}}
#define OP_JNE(_opd)  {IR_OPD_JNE,  _opd, {}}

#define OP_GLOBAL_LABEL(_name) {IR_OPC_GLOBAL_LABEL, OPD_GLOBAL_LABEL(_name)}
#define OP_LOCAL_LABEL(_name)  {IR_OPC_LOCAL_LABEL,  OPD_LOCAL_LABEL(_name)}

#define OP_NOP     {IR_OPC_NOP, {}, {}}
#define OP_RET     {IR_OPC_RET, {}, {}}
#define OP_SYSCALL {IR_OP_SYSCALL, {}, {}}

//——————————————————————————————————————————————————————————————————————————————

#define EMIT(_ir_instr) ir_emit_instr(ctx->ir_buf, _ir_instr)

//——————————————————————————————————————————————————————————————————————————————

#endif // _IR_DSL_H__

//——————————————————————————————————————————————————————————————————————————————
