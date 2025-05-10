#ifndef _IR_H__
#define _IR_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang.h"
#include <inttypes.h>

#define IR_BUFFER_INIT_CAPACITY 64

#define REX_PREFIX      0b0100
#define OPERANDS_64_BIT 1

//——————————————————————————————————————————————————————————————————————————————

enum reg_t
{
    IR_REG_RAX = 0,  // Return value
    IR_REG_RSP = 4,  // Stck pointer
    IR_REG_RBP = 5,  // Base pointer for local vars
    IR_REG_R10 = 10, // Temporary 1
    IR_REG_R11 = 11, // Temporary 2
    IR_REG_NAN = 16, // Invalid
};

const char* const RegNames[] =
{
    "rax",
    "rcx",
    "rdx",
    "rbx",
    "rsp",
    "rbp",
    "rsi",
    "rdi",
    "r8",
    "r9",
    "r10",
    "r11",
    "r12",
    "r13",
    "r14",
    "r15",
    "invalid"
};

//——————————————————————————————————————————————————————————————————————————————

enum opcode_t
{
    IR_OPCODE_NOP          = 0,
    IR_OPCODE_ADD          = 1,
    IR_OPCODE_SUB          = 2,
    IR_OPCODE_MUL          = 3,
    IR_OPCODE_DIV          = 4,
    IR_OPCODE_MOV          = 5,
    IR_OPCODE_PUSH         = 6,
    IR_OPCODE_POP          = 7,
    IR_OPCODE_CALL         = 8,
    IR_OPCODE_RET          = 9,
    IR_OPCODE_SYSCALL      = 10,
    IR_OPCODE_JMP          = 11,
    IR_OPCODE_JE           = 12,
    IR_OPCODE_JNE          = 13,
    IR_OPCODE_TEST         = 14,
    IR_OPCODE_LOCAL_LABEL  = 15,
    IR_OPCODE_GLOBAL_LABEL = 16,
};

//——————————————————————————————————————————————————————————————————————————————

enum operand_type_t
{
    IR_OPERAND_NAN          = 0,
    IR_OPERAND_REGISTER     = 1,
    IR_OPERAND_MEMORY       = 2,
    IR_OPERAND_IMMERSIVE    = 3,
    IR_OPERAND_GLOBAL_LABEL = 4,
    IR_OPERAND_LOCAL_LABEL  = 5,
};

//——————————————————————————————————————————————————————————————————————————————

union operand_value_t
{
    reg_t       reg;
    int         addr;
    number_t    imm;
    const char* name;
    uint64_t    label_number;
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

struct opcode_info_t
{
    const char* asm_name;
    lang_status_t (*asm_func)    (lang_ctx_t* ctx, ir_instr_t* instr);
    lang_status_t (*encode_func) (lang_ctx_t* ctx, ir_instr_t* instr);
};

//——————————————————————————————————————————————————————————————————————————————

lang_status_t zeroary_opcode_IR_to_asm(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t unary_opcode_IR_to_asm (lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t binary_opcode_IR_to_asm (lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t IR_to_asm_local_label (lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t IR_to_asm_global_label(lang_ctx_t* ctx, ir_instr_t* instr);

lang_status_t IR_to_asm(lang_ctx_t* ctx);

//——————————————————————————————————————————————————————————————————————————————

lang_status_t encode_nop(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_add(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_sub(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_mul(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_div(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_mov(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_push(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_pop(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_call(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_ret(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_syscall(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_jmp(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_je(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_jne(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_test(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_local_label(lang_ctx_t* ctx, ir_instr_t* instr);
lang_status_t encode_global_label(lang_ctx_t* ctx, ir_instr_t* instr);

//——————————————————————————————————————————————————————————————————————————————

const opcode_info_t OpcodesTable[] =
{
    {"nop",          &zeroary_opcode_IR_to_asm, &encode_nop},
    {"add",          &binary_opcode_IR_to_asm,  &encode_add},
    {"sub",          &binary_opcode_IR_to_asm,  &encode_sub},
    {"mul",          &binary_opcode_IR_to_asm,  &encode_mul},
    {"div",          &binary_opcode_IR_to_asm,  &encode_div},
    {"mov",          &binary_opcode_IR_to_asm,  &encode_mov},
    {"push qword",   &unary_opcode_IR_to_asm,   &encode_push},
    {"pop qword",    &unary_opcode_IR_to_asm,   &encode_pop},
    {"call",         &unary_opcode_IR_to_asm,   &encode_call},
    {"ret",          &zeroary_opcode_IR_to_asm, &encode_ret},
    {"syscall",      &zeroary_opcode_IR_to_asm, &encode_syscall},
    {"jmp",          &unary_opcode_IR_to_asm,   &encode_jmp},
    {"je",           &unary_opcode_IR_to_asm,   &encode_je},
    {"jne",          &unary_opcode_IR_to_asm,   &encode_jne},
    {"test",         &binary_opcode_IR_to_asm,  &encode_test},
    {"local_label",  &zeroary_opcode_IR_to_asm, &encode_local_label},
    {"global_label", &zeroary_opcode_IR_to_asm, &encode_global_label},
};

//——————————————————————————————————————————————————————————————————————————————

struct rex_t {
    uint8_t b : 1;
    uint8_t x : 1;
    uint8_t r : 1;
    uint8_t w : 1;
    uint8_t prefix : 4;
};

struct modrm_t {
    uint8_t rm  : 3;
    uint8_t reg : 3;
    uint8_t mod : 2;
};

enum instr_type_t
{
    INSTR_TYPE_REG_REG,   // reg ← reg
    INSTR_TYPE_REG_MEM,   // reg ← [rbp ± offset]
    INSTR_TYPE_MEM_REG,   // [rbp ± offset] ← reg
    INSTR_TYPE_REG_IMM,   // reg ← imm
    INSTR_TYPE_MEM_IMM,   // [rbp ± offset] ← imm
    INSTR_TYPE_UNDEFINED,
};

struct encoded_instr_info_t {
    bool    has_rex;
    bool    has_modrm;
    bool    has_disp;
    bool    has_imm;
    uint8_t disp_size;
    uint8_t imm_size;
    uint8_t opcode_size;
}

struct encoded_instr_t {
    rex_t                rex;
    uint16_t             opcode;
    modrm_t              modrm;
    int32_t              disp;
    uint64_t             imm;
    encoded_instr_info_t info;
};

//——————————————————————————————————————————————————————————————————————————————

lang_status_t ir_ctx_ctor(ir_ctx_t* ctx, size_t init_capacity);
lang_status_t ir_ctx_dtor(ir_ctx_t* ctx);
lang_status_t ir_emit_operation(ir_ctx_t* ctx, ir_instr_t instr);

//==============================================================================

operand_t operand_register(reg_t reg);
operand_t operand_memory(int addr);
operand_t operand_immersive(int64_t imm);
operand_t operand_global_label(const char* name);
operand_t operand_local_label(uint64_t label_number);

lang_status_t emit_nop(lang_ctx_t* ctx);
lang_status_t emit_add(lang_ctx_t* ctx, operand_t opd1, operand_t opd2);
lang_status_t emit_sub(lang_ctx_t* ctx, operand_t opd1, operand_t opd2);
lang_status_t emit_mul(lang_ctx_t* ctx, operand_t opd1, operand_t opd2);
lang_status_t emit_div(lang_ctx_t* ctx, operand_t opd1, operand_t opd2);
lang_status_t emit_mov(lang_ctx_t* ctx, operand_t src, operand_t dst);
lang_status_t emit_push(lang_ctx_t* ctx, operand_t opd);
lang_status_t emit_pop(lang_ctx_t* ctx, operand_t opd);
lang_status_t emit_call(lang_ctx_t* ctx, const char* name);
lang_status_t emit_ret(lang_ctx_t* ctx);
lang_status_t emit_global_label(lang_ctx_t* ctx, const char* name);
lang_status_t emit_local_label(lang_ctx_t* ctx, int64_t label_number);
lang_status_t emit_jmp(lang_ctx_t* ctx, int64_t label_number);
lang_status_t emit_je(lang_ctx_t* ctx, int64_t label_number);
lang_status_t emit_jne(lang_ctx_t* ctx, int64_t label_number);
lang_status_t emit_syscall(lang_ctx_t* ctx);
lang_status_t emit_test(lang_ctx_t* ctx, operand_t opd1, operand_t opd2);
lang_status_t emit_binary_operation(lang_ctx_t* ctx,
                                    operator_code_t opcode,
                                    operand_t opd1,
                                    operand_t opd2);

//——————————————————————————————————————————————————————————————————————————————

#endif // _IR_H__
