#include "ir.h"
#include "custom_assert.h"

//—————————————————————————————————————————————————————————————————————————————

lang_status_t ir_ctx_ctor(ir_ctx_t* ctx, size_t init_capacity)
{
    ASSERT(ctx);

    //--------------------------------------------------------------------------

    ctx->buffer = (ir_instr_t*) calloc(init_capacity, sizeof(ir_instr_t));

    if (!ctx->buffer) {
        return LANG_STD_ALLOCATE_ERROR;
    }

    ctx->buffer_capacity = init_capacity;
    ctx->buffer_size = 0;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t ir_ctx_dtor(ir_ctx_t* ctx)
{
    ASSERT(ctx);

    //--------------------------------------------------------------------------

    if (!ctx->buffer) {
        return LANG_ERROR;
    }

    free(ctx->buffer);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t ir_emit_operation(ir_ctx_t* ctx, ir_instr_t instr)
{
    ASSERT(ctx);

    //--------------------------------------------------------------------------

    if (ctx->buffer_size >= ctx->buffer_capacity) {
        ctx->buffer_capacity *= 2;
        ctx->buffer = (ir_instr_t*) realloc(ctx->buffer,
                                            ctx->buffer_capacity *
                                            sizeof(ir_instr_t));
    }

    if (!ctx->buffer) {
        return LANG_ERROR;
    }

    ctx->buffer[ctx->buffer_size++] = instr;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

operand_t operand_register(reg_t reg)
{
    return (operand_t) {.type  = IR_OPERAND_REGISTER,
                        .value = {.reg = reg}};
}

//==============================================================================

operand_t operand_memory(int addr)
{
    return (operand_t) {.type  = IR_OPERAND_MEMORY,
                        .value = {.addr = addr}};
}

//==============================================================================

operand_t operand_immersive(int64_t imm)
{
    return (operand_t) {.type  = IR_OPERAND_IMMERSIVE,
                        .value = {.imm = imm}};
}

//==============================================================================

operand_t operand_label(const char* name)
{
    return (operand_t) {.type  = IR_OPERAND_LABEL,
                        .value = {.name = name}};
}

//==============================================================================

lang_status_t emit_nop(lang_ctx_t* ctx)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_NOP, {}, {}});
}

//==============================================================================

lang_status_t emit_binary_operation(lang_ctx_t* ctx,
                                    operator_code_t opcode,
                                    operand_t opd1,
                                    operand_t opd2)
{
    opcode_t IR_opcode = IR_OPCODE_NOP;

    switch (opcode) {
        case ADD: {
            IR_opcode = IR_OPCODE_ADD;
            break;
        }
        case SUB: {
            IR_opcode = IR_OPCODE_SUB;
            break;
        }
        case MUL: {
            IR_opcode = IR_OPCODE_MUL;
            break;
        }
        case DIV: {
            IR_opcode = IR_OPCODE_DIV;
            break;
        }
        default: {
            return LANG_ERROR;
        }
    }

    return ir_emit_operation(ctx->ir_ctx,
                             {IR_opcode, opd1, opd2});
}

//==============================================================================

lang_status_t emit_add(lang_ctx_t* ctx, operand_t opd1, operand_t opd2)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_ADD, opd1, opd2});
}

//==============================================================================

lang_status_t emit_sub(lang_ctx_t* ctx, operand_t opd1, operand_t opd2)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_SUB, opd1, opd2});
}

//==============================================================================

lang_status_t emit_mul(lang_ctx_t* ctx, operand_t opd1, operand_t opd2)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_MUL, opd1, opd2});
}

//==============================================================================

lang_status_t emit_div(lang_ctx_t* ctx, operand_t opd1, operand_t opd2)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_DIV, opd1, opd2});
}

//==============================================================================

lang_status_t emit_mov(lang_ctx_t* ctx, operand_t dst, operand_t src)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_MOV, dst, src});
}

//==============================================================================

lang_status_t emit_push(lang_ctx_t* ctx, operand_t src)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_PUSH, src, {}});
}

//==============================================================================

lang_status_t emit_pop(lang_ctx_t* ctx, operand_t dst)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_POP, dst, {}});
}

//==============================================================================

lang_status_t emit_call(lang_ctx_t* ctx, const char* name)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_CALL, operand_label(name), {}});
}

//==============================================================================

lang_status_t emit_ret(lang_ctx_t* ctx)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_RET, {}, {}});
}

//==============================================================================

lang_status_t emit_syscall(lang_ctx_t* ctx)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_SYSCALL, {}, {}});
}

//==============================================================================

lang_status_t emit_func(lang_ctx_t* ctx, const char* name)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_LABEL, operand_label(name), {}});
}

//==============================================================================

lang_status_t emit_label(lang_ctx_t* ctx, int64_t label_number)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_LABEL, operand_immersive(label_number), {}});
}

//==============================================================================

lang_status_t emit_jmp(lang_ctx_t* ctx, int64_t label_number)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_JMP, operand_immersive(label_number), {}});
}

//==============================================================================

lang_status_t emit_je(lang_ctx_t* ctx, int64_t label_number)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_JE, operand_immersive(label_number), {}});
}

//==============================================================================

lang_status_t emit_jne(lang_ctx_t* ctx, int64_t label_number)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_JNE, operand_immersive(label_number), {}});
}

//==============================================================================

lang_status_t emit_test(lang_ctx_t* ctx, operand_t opd1, operand_t opd2)
{
    return ir_emit_operation(ctx->ir_ctx,
                             {IR_OPCODE_TEST, opd1, opd2});
}

//==============================================================================
