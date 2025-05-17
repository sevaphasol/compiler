#ifndef _IR_INSTR_H__
#define _IR_INSTR_H__

//——————————————————————————————————————————————————————————————————————————————

#include "ir_opcodes.h"
#include "ir_operands.h"

//——————————————————————————————————————————————————————————————————————————————

struct ir_instr_t {
    ir_opc_t opc;
    ir_opd_t opd1;
    ir_opd_t opd2;
};

//——————————————————————————————————————————————————————————————————————————————

enum ir_instr_type_t {
    IR_INSTR_TYPE_REG_REG   = 0,   // reg ← reg
    IR_INSTR_TYPE_REG_MEM   = 1,   // reg ← [rbp ± offset]
    IR_INSTR_TYPE_MEM_REG   = 2,   // [rbp ± offset] ← reg
    IR_INSTR_TYPE_REG_IMM   = 3,   // reg ← imm
    IR_INSTR_TYPE_MEM_IMM   = 4,   // [rbp ± offset] ← imm
    IR_INSTR_TYPE_UNDEFINED = 5,
};

//——————————————————————————————————————————————————————————————————————————————

#endif //_IR_INSTR_H__
