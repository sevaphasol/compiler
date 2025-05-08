#ifndef _IR_H__
#define _IR_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang.h"
#include <inttypes.h>

#define IR_BUFFER_INIT_CAPACITY 64

//——————————————————————————————————————————————————————————————————————————————

enum reg_t
{
    REG_NAN = -1, // Invalid
    REG_RAX = 0,  // Return value
    REG_RSP = 4,  // Stck pointer
    REG_RBP = 5,  // Base pointer for local vars
    REG_R10 = 10, // Temporary 1
    REG_R11 = 11, // Temporary 2
};

//——————————————————————————————————————————————————————————————————————————————

enum opcode_t
{
    OP_NOP     = 0,
    OP_ADD     = 1,
    OP_SUB     = 2,
    OP_MUL     = 3,
    OP_DIV     = 4,
    OP_MOV     = 5,
    OP_PUSH    = 6,
    OP_POP     = 7,
    OP_CALL    = 8,
    OP_RET     = 9,
    OP_SYSCALL = 10,
    OP_LABEL   = 11,
    OP_JMP     = 12,
    OP_JE      = 13,
    OP_JNE     = 14,
    OP_TEST    = 15,
};

//——————————————————————————————————————————————————————————————————————————————

enum operand_type_t
{
    OPD_NAN = 0,
    OPD_REG = 1,
    OPD_MEM = 2,
    OPD_IMM = 3,
    OPD_LBL = 4, // label
};

//——————————————————————————————————————————————————————————————————————————————

union operand_value_t
{
    reg_t    reg;
    int      addr;
    number_t imm;
    const char* name;
};

//——————————————————————————————————————————————————————————————————————————————

struct operand_t
{
    operand_type_t  type;
    operand_value_t value;
};

//——————————————————————————————————————————————————————————————————————————————

struct ir_instr_t
{
    opcode_t  op;
    operand_t opd1;
    operand_t opd2;
};

//——————————————————————————————————————————————————————————————————————————————

struct ir_ctx_t
{
    ir_instr_t* buffer;
    size_t      buffer_size;
    size_t      buffer_capacity;
};

//——————————————————————————————————————————————————————————————————————————————

lang_status_t ir_ctx_ctor(ir_ctx_t* ctx, size_t init_capacity);
lang_status_t ir_ctx_dtor(ir_ctx_t* ctx);
lang_status_t ir_emit(ir_ctx_t* ctx, ir_instr_t instr);

//==============================================================================

operand_t opd_reg(reg_t reg);
operand_t opd_mem(int addr);
operand_t opd_imm(uint64_t imm);
operand_t opd_lbl(const char* name);
lang_status_t ir_emit_nop(lang_ctx_t* ctx);
lang_status_t ir_emit_add(lang_ctx_t* ctx, operand_t opd1, operand_t opd2);
lang_status_t ir_emit_sub(lang_ctx_t* ctx, operand_t opd1, operand_t opd2);
lang_status_t ir_emit_mul(lang_ctx_t* ctx, operand_t opd1, operand_t opd2);
lang_status_t ir_emit_div(lang_ctx_t* ctx, operand_t opd1, operand_t opd2);
lang_status_t ir_emit_mov(lang_ctx_t* ctx, operand_t src, operand_t dst);
lang_status_t ir_emit_push(lang_ctx_t* ctx, operand_t opd);
lang_status_t ir_emit_pop(lang_ctx_t* ctx, operand_t opd);
lang_status_t ir_emit_call(lang_ctx_t* ctx, const char* name);
lang_status_t ir_emit_ret(lang_ctx_t* ctx);
lang_status_t ir_emit_label(lang_ctx_t* ctx, const char* name);
lang_status_t ir_emit_jmp(lang_ctx_t* ctx, const char* name);
lang_status_t ir_emit_je(lang_ctx_t* ctx, const char* name);
lang_status_t ir_emit_jne(lang_ctx_t* ctx, const char* name);
lang_status_t ir_emit_syscall(lang_ctx_t* ctx);
lang_status_t ir_emit_test(lang_ctx_t* ctx, operand_t opd);

//——————————————————————————————————————————————————————————————————————————————

#endif // _IR_H__
