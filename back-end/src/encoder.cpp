//——————————————————————————————————————————————————————————————————————————————

#include "encoder.h"
#include "custom_assert.h"
#include "buffer.h"
#include "IR.h"

//——————————————————————————————————————————————————————————————————————————————

lang_status_t encode_instr(ir_instr_t*  ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    return (*OpcodesTable[ir_instr->op].encode_func)(ctx, ir_instr, encoded_instr);
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_encoded_instr(buffer_t* bin_buf, const encoded_instr_t* instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    if (instr->info.has_rex) {
        buffer_append(ctx->byte_code_ctx, (char*) &instr->rex, sizeof(instr->rex));
    }

    buffer_append(ctx->byte_code_ctx, (char*) &instr->opcode, instr->info.opcode_size);

    if (instr->info.has_modrm) {
        buffer_append(ctx->byte_code_ctx, (char*) &instr->modrm, sizeof(instr->modrm));
    }

    if (instr->info.has_disp) {
        buffer_append(ctx->byte_code_ctx, (char*) &instr->disp, instr->info.disp_size);
    }

    if (instr->info.has_imm) {
        buffer_append(ctx->byte_code_ctx, (char*) &instr->imm, instr->info.imm_size);
    }

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t IR_to_byte_code(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    size_t size = ctx->ir_ctx->buffer_size;

    for (size_t i = 0; i < size; i++) {
        ir_instr_t ir_instr = ctx->ir_ctx->buffer[i];
        encoded_instr_t encoded_instr = {};
        encode_instr(ctx, &ir_instr, &encoded_instr);
        emit_encoded_instr(ctx, &encoded_instr);
    }

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————
