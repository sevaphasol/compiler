#ifndef X86_64_OPC_TABLE_H__
#define X86_64_OPC_TABLE_H__

//——————————————————————————————————————————————————————————————————————————————

#include "x86_64_codes.h"
#include "ir_opcodes.h"
#include "ir_instr.h"
#include <inttypes.h>

//——————————————————————————————————————————————————————————————————————————————

struct bin_instr_x86_64_info_t {
    x86_64_opcode_t opc;
    uint8_t         modrm_reg;
};

//——————————————————————————————————————————————————————————————————————————————

const bin_instr_x86_64_info_t AddOpcInfo[] = {
    [IR_INSTR_TYPE_REG_REG] = {.opc = X86_64_ADD_RR_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_REG_MEM] = {.opc = X86_64_ADD_RM_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_MEM_REG] = {.opc = X86_64_ADD_MR_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_REG_IMM] = {.opc = X86_64_ADD_RI_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_MEM_IMM] = {.opc = X86_64_ADD_MI_OPCODE, .modrm_reg = 0}
};

//——————————————————————————————————————————————————————————————————————————————

const bin_instr_x86_64_info_t SubOpcInfo[] = {
    [IR_INSTR_TYPE_REG_REG] = {.opc = X86_64_SUB_RR_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_REG_MEM] = {.opc = X86_64_SUB_RM_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_MEM_REG] = {.opc = X86_64_SUB_MR_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_REG_IMM] = {.opc = X86_64_SUB_RI_OPCODE, .modrm_reg = 5},
    [IR_INSTR_TYPE_MEM_IMM] = {.opc = X86_64_SUB_MI_OPCODE, .modrm_reg = 5}
};

//——————————————————————————————————————————————————————————————————————————————

const bin_instr_x86_64_info_t MovOpcInfo[] = {
    [IR_INSTR_TYPE_REG_REG] = {.opc = X86_64_MOV_RR_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_REG_MEM] = {.opc = X86_64_MOV_RM_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_MEM_REG] = {.opc = X86_64_MOV_MR_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_REG_IMM] = {.opc = X86_64_MOV_RI_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_MEM_IMM] = {.opc = X86_64_MOV_MI_OPCODE, .modrm_reg = 0}
};

//——————————————————————————————————————————————————————————————————————————————

const bin_instr_x86_64_info_t TestOpcInfo[] = {
    [IR_INSTR_TYPE_REG_REG] = {.opc = X86_64_TEST_RR_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_REG_MEM] = {.opc = X86_64_TEST_RM_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_MEM_REG] = {.opc = X86_64_TEST_MR_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_REG_IMM] = {.opc = X86_64_TEST_RI_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_MEM_IMM] = {.opc = X86_64_TEST_MI_OPCODE, .modrm_reg = 0}
};

//——————————————————————————————————————————————————————————————————————————————

const bin_instr_x86_64_info_t ImulOpcInfo[] = {
    [IR_INSTR_TYPE_REG_REG] = {.opc = X86_64_IMUL_RR_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_REG_MEM] = {.opc = X86_64_IMUL_RM_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_MEM_REG] = {.opc = X86_64_INVALID_OPCODE, .modrm_reg = 0},
    [IR_INSTR_TYPE_REG_IMM] = {.opc = X86_64_IMUL_RI_OPCODE, .modrm_reg = 0},
};

//——————————————————————————————————————————————————————————————————————————————

#define _INSTR_OPC_INFO sizeof(TestOpcInfo)

//——————————————————————————————————————————————————————————————————————————————

// /* Table of tables with info about opc and \r */
// const bin_instr_x86_64_info_t* OpcInfo[] = {
//     [IR_OPC_ADD]  = AddOpcInfo,
//     [IR_OPC_SUB]  = SubOpcInfo,
//     [IR_OPC_MOV]  = MovOpcInfo,
//     [IR_OPC_TEST] = TestOpcInfo
// };

//——————————————————————————————————————————————————————————————————————————————

#endif // X86_64_OPC_TABLE_H__
