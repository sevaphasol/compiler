#include "IR.h"
#include "custom_assert.h"

#define _DSL_DEFINE_
#include "dsl.h"

//——————————————————————————————————————————————————————————————————————————————

lang_status_t IR_to_asm(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    //--------------------------------------------------------------------------

    RAW_ASM("section .text\n");
    RAW_ASM("global _start\n\n");

    size_t size = ctx->ir_ctx->buffer_size;

    for (size_t i = 0; i < size; i++) {
        ir_instr_t instr = ctx->ir_ctx->buffer[i];
        (*OpcodesTable[instr.op].asm_func)(ctx, &instr);
    }

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t zeroary_opcode_IR_to_asm(lang_ctx_t* ctx, ir_instr_t* instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    opcode_t opcode = instr->op;

    if (opcode == IR_OPCODE_LOCAL_LABEL) {
        RAW_ASM(".L%ld:\n", instr->opd1.value.label_number);
    } else if (opcode == IR_OPCODE_GLOBAL_LABEL) {
        RAW_ASM("%s:\n", instr->opd1.value.name);
    } else {
        TAB_ASM("%s\n", OpcodesTable[instr->op].asm_name);
    }

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t operand_IR_to_asm(lang_ctx_t* ctx, operand_t* operand)
{
    ASSERT(ctx);

    //--------------------------------------------------------------------------

    switch(operand->type) {
        case IR_OPERAND_REGISTER: {
            RAW_ASM("%s", RegNames[operand->value.reg]);
            break;
        }
        case IR_OPERAND_IMMERSIVE: {
            RAW_ASM("%ld", operand->value.imm);
            break;
        }
        case IR_OPERAND_MEMORY: {
            if (operand->value.addr < 0) {
                RAW_ASM("[rbp + %d]", -operand->value.addr)
            } else {
                RAW_ASM("[rbp - %d]",  operand->value.addr);
            }
            break;
        }
        case IR_OPERAND_LOCAL_LABEL: {
            RAW_ASM(".L%ld", operand->value.label_number);
            break;
        }
        case IR_OPERAND_GLOBAL_LABEL: {
            RAW_ASM("%s", operand->value.name);
            break;
        }
        default: {
            return LANG_ERROR;
        }
    }

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t unary_opcode_IR_to_asm(lang_ctx_t* ctx, ir_instr_t* instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    TAB_ASM("%s ", OpcodesTable[instr->op].asm_name);
    operand_IR_to_asm(ctx, &instr->opd1);
    RAW_ASM("\n");

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t binary_opcode_IR_to_asm(lang_ctx_t* ctx, ir_instr_t* instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    TAB_ASM("%s ", OpcodesTable[instr->op].asm_name);
    operand_IR_to_asm(ctx, &instr->opd1);
    RAW_ASM(", ");
    operand_IR_to_asm(ctx, &instr->opd2);
    RAW_ASM("\n");

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

#define _DSL_UNDEF_
#include "dsl.h"

//——————————————————————————————————————————————————————————————————————————————
