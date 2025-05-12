//——————————————————————————————————————————————————————————————————————————————

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
    INSTR_TYPE_REG_REG,   // reg ← reg
    INSTR_TYPE_REG_MEM,   // reg ← [rbp ± offset]
    INSTR_TYPE_MEM_REG,   // [rbp ± offset] ← reg
    INSTR_TYPE_REG_IMM,   // reg ← imm
    INSTR_TYPE_MEM_IMM,   // [rbp ± offset] ← imm
    INSTR_TYPE_UNDEFINED,
};

//——————————————————————————————————————————————————————————————————————————————

#endif //_IR_INSTR_H__

//——————————————————————————————————————————————————————————————————————————————
