//——————————————————————————————————————————————————————————————————————————————

#ifndef _IR_TABLES_H__
#define _IR_TABLES_H__

//——————————————————————————————————————————————————————————————————————————————

#include "ir_opcodes.h"
#include "ir_funcs.h"

//——————————————————————————————————————————————————————————————————————————————

/* IR operation asm names. Need this for building .s file (-s flag). */
const char* const IrOpAsmNamesTable[] = {
    [IR_OPC_NOP]          = "nop",
    [IR_OPC_ADD]          = "add",
    [IR_OPC_SUB]          = "sub",
    [IR_OPC_MUL]          = "mul",
    [IR_OPC_DIV]          = "div",
    [IR_OPC_MOV]          = "mov",
    [IR_OPC_PUSH]         = "push qword",
    [IR_OPC_POP]          = "pop qword",
    [IR_OPC_CALL]         = "call",
    [IR_OPC_RET]          = "ret",
    [IR_OPC_SYSCALL]      = "syscall",
    [IR_OPC_JMP]          = "jmp",
    [IR_OPC_JE]           = "je",
    [IR_OPC_JNE]          = "jne",
    [IR_OPC_TEST]         = "test",
    [IR_OPC_LOCAL_LABEL]  = "local_label",
    [IR_OPC_GLOBAL_LABEL] = "global_label",
};

//——————————————————————————————————————————————————————————————————————————————

typedef lang_status_t (*asm_func_t)(lang_ctx_t* ctx, ir_instr_t* ir_instr);

const asm_func_t IrOpAsmFuncsTable[] = {
    [IR_OPC_NOP]          = asm_nop,
    [IR_OPC_ADD]          = asm_add,
    [IR_OPC_SUB]          = asm_sub,
    [IR_OPC_MUL]          = asm_mul,
    [IR_OPC_DIV]          = asm_div,
    [IR_OPC_MOV]          = asm_mov,
    [IR_OPC_PUSH]         = asm_push,
    [IR_OPC_POP]          = asm_pop,
    [IR_OPC_CALL]         = asm_call,
    [IR_OPC_RET]          = asm_ret,
    [IR_OPC_SYSCALL]      = asm_syscall,
    [IR_OPC_JMP]          = asm_jmp,
    [IR_OPC_JE]           = asm_je,
    [IR_OPC_JNE]          = asm_jne,
    [IR_OPC_TEST]         = asm_test,
    [IR_OPC_LOCAL_LABEL]  = asm_local_label,
    [IR_OPC_GLOBAL_LABEL] = asm_global_label,
};

//——————————————————————————————————————————————————————————————————————————————

typedef lang_status_t (*encode_func_t)(lang_ctx_t*  ctx,
                                       ir_instr_t*  ir_instr,
                                       bin_instr_t* bin_instr);

const encode_func_t IrOpEncodeFuncsTable[] = {
    [IR_OPC_NOP]          = encode_nop,
    [IR_OPC_ADD]          = encode_add,
    [IR_OPC_SUB]          = encode_sub,
    [IR_OPC_MUL]          = encode_mul,
    [IR_OPC_DIV]          = encode_div,
    [IR_OPC_MOV]          = encode_mov,
    [IR_OPC_PUSH]         = encode_push,
    [IR_OPC_POP]          = encode_pop,
    [IR_OPC_CALL]         = encode_call,
    [IR_OPC_RET]          = encode_ret,
    [IR_OPC_SYSCALL]      = encode_syscall,
    [IR_OPC_JMP]          = encode_jmp,
    [IR_OPC_JE]           = encode_je,
    [IR_OPC_JNE]          = encode_jne,
    [IR_OPC_TEST]         = encode_test,
    [IR_OPC_LOCAL_LABEL]  = encode_local_label,
    [IR_OPC_GLOBAL_LABEL] = encode_global_label,
};

//——————————————————————————————————————————————————————————————————————————————

#endif // _IR_TABLES_H__

//——————————————————————————————————————————————————————————————————————————————
