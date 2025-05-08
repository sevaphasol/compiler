#include "ir.h"
#include "custom_assert.h"

//—————————————————————————————————————————————————————————————————————————————

lang_status_t ir_ctx_ctor(ir_ctx_t* ctx, size_t init_capacity)
{
    ASSERT(ctx);

    ctx->buffer = (ir_instr_t*) calloc(init_capacity, sizeof(ir_instr_t));

    if (!ctx->buffer) {
        return LANG_STD_ALLOCATE_ERROR;
    }

    ctx->buffer_capacity = init_capacity;
    ctx->buffer_size = 0;

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t ir_ctx_dtor(ir_ctx_t* ctx)
{
    ASSERT(ctx);

    if (!ctx->buffer) {
        return LANG_ERROR;
    }

    free(ctx->buffer);

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t ir_emit(ir_ctx_t* ctx, ir_instr_t instr)
{
    ASSERT(ctx);

    if (ctx->buffer_size >= ctx->buffer_capacity) {
        ctx->buffer_capacity *= 2;
        ctx->buffer = (ir_instr_t*) realloc(ctx->buffer,
                                            ctx->buffer_capacity *
                                            sizeof(ir_instr_t));
    }

    ctx->buffer[ctx->buffer_size++] = instr;

    return LANG_SUCCESS;
}

//==============================================================================

operand_t opd_reg(reg_t reg)
{
    return (operand_t) {.type  = OPD_REG,
                        .value = {.reg = reg}};
}

//==============================================================================

operand_t opd_mem(int addr)
{
    return (operand_t) {.type  = OPD_MEM,
                        .value = {.addr = addr}};
}

//==============================================================================

operand_t opd_imm(uint64_t imm)
{
    return (operand_t) {.type  = OPD_IMM,
                        .value = {.imm = imm}};
}

//==============================================================================

operand_t opd_lbl(const char* name)
{
    return (operand_t) {.type  = OPD_LBL,
                        .value = {.name = name}};
}

//==============================================================================

lang_status_t ir_emit_nop(lang_ctx_t* ctx)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t) {OP_NOP, {}, {}});
}

//==============================================================================

lang_status_t ir_emit_add(lang_ctx_t* ctx, operand_t opd1, operand_t opd2)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t)
                   {OP_ADD, opd1, opd2});
}

//==============================================================================

lang_status_t ir_emit_sub(lang_ctx_t* ctx, operand_t opd1, operand_t opd2)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t)
                   {OP_SUB, opd1, opd2});
}

//==============================================================================

lang_status_t ir_emit_mul(lang_ctx_t* ctx, operand_t opd1, operand_t opd2)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t)
                   {OP_MUL, opd1, opd2});
}

//==============================================================================

lang_status_t ir_emit_div(lang_ctx_t* ctx, operand_t opd1, operand_t opd2)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t)
                   {OP_DIV, opd1, opd2});
}

//==============================================================================

lang_status_t ir_emit_mov(lang_ctx_t* ctx, operand_t src, operand_t dst)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t) {OP_MOV, src, dst});
}

//==============================================================================

lang_status_t ir_emit_push(lang_ctx_t* ctx, operand_t opd)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t) {OP_PUSH, opd, {}});
}

//==============================================================================

lang_status_t ir_emit_pop(lang_ctx_t* ctx, operand_t opd)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t) {OP_POP, opd, {}});
}

//==============================================================================

lang_status_t ir_emit_call(lang_ctx_t* ctx, const char* name)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t) {OP_CALL, opd_lbl(name), {}});
}

//==============================================================================

lang_status_t ir_emit_ret(lang_ctx_t* ctx)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t) {OP_RET, {}, {}});
}

//==============================================================================

lang_status_t ir_emit_syscall(lang_ctx_t* ctx)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t) {OP_SYSCALL, {}, {}});
}

//==============================================================================

lang_status_t ir_emit_func(lang_ctx_t* ctx, const char* name)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t) {OP_LABEL, opd_lbl(name), {}});
}

//==============================================================================

lang_status_t ir_emit_label(lang_ctx_t* ctx, int64_t label_number)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t) {OP_LABEL, opd_imm(label_number), {}});
}

//==============================================================================

lang_status_t ir_emit_jmp(lang_ctx_t* ctx, int64_t label_number)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t) {OP_JMP, opd_imm(label_number), {}});
}

//==============================================================================

lang_status_t ir_emit_je(lang_ctx_t* ctx, int64_t label_number)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t) {OP_JE, opd_imm(label_number), {}});
}

//==============================================================================

lang_status_t ir_emit_jne(lang_ctx_t* ctx, int64_t label_number)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t) {OP_JNE, opd_imm(label_number), {}});
}

//==============================================================================

lang_status_t ir_emit_test(lang_ctx_t* ctx, operand_t opd)
{
    return ir_emit(ctx->ir_ctx, (ir_instr_t) {OP_TEST, opd, {}});
}

//==============================================================================
