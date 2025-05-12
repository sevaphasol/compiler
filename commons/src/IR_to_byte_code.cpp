#include <string.h>

#include "IR.h"
#include "custom_assert.h"
#include "x86_64_opcodes.h"
#include "buffer.h"

//——————————————————————————————————————————————————————————————————————————————

lang_status_t encode_instr(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(encoded_instr);

    return (*OpcodesTable[ir_instr->op].encode_func)(ctx, ir_instr, encoded_instr);
}

//==============================================================================

lang_status_t emit_encoded_instr(buffer_t* bin_buf, const bin_instr_t* instr)
{
    ASSERT(bin_buf);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    if (instr->info.has_rex)
        buf_write(bin_buf, &instr->rex, sizeof(instr->rex));

    buf_write(bin_buf, &instr->opcode, instr->info.opcode_size);

    if (instr->info.has_modrm)
        buf_write(bin_buf, &instr->modrm, sizeof(instr->modrm));

    if (instr->info.has_disp)
        buf_write(bin_buf, &instr->disp, instr->info.disp_size);

    if (instr->info.has_imm)
        buf_write(bin_buf, &instr->imm, instr->info.imm_size);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t IR_to_byte_code(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    //--------------------------------------------------------------------------

    size_t size = ctx->ir_ctx->buffer_size;

    for (size_t i = 0; i < size; i++) {
        ir_instr_t ir_instr = ctx->ir_ctx->buffer[i];
        encoded_instr_t encoded_instr = {};
        encode_instr(ctx, &ir_instr, &encoded_instr);
        emit_encoded_instr(ctx, &encoded_instr);
    }

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

uint8_t choose_optimal_disp(int32_t offset, uint8_t* mod)
{
    if (offset >= -128 && offset <= 127) {
        *mod = 0b01;
        return 1;
    } else {
        *mod = 0b10;
        return 4;
    }
}

//==============================================================================

modrm_t build_modrm(uint8_t rm, uint8_t reg, uint8_t mod)
{
    return (modrm_t) {
        .mod = mod,
        .reg = reg,
        .rm  = rm
    };
}

//==============================================================================

rex_t build_rex(uint8_t w, uint8_t r, uint8_t b)
{
    return (rex_t) {
        .prefix = 0b0100, // fixed constant
        .w      = w,
        .r      = r,
        .x      = 0, // SIB is never used
        .b      = b
    };
}

//==============================================================================

lang_status_t setup_rex_modrm_rr(encoded_instr_t* encoded_instr,
                                 ir_instr_t*      ir_instr)
{
    ASSERT(encoded_instr);
    ASSERT(ir_instr);

    //--------------------------------------------------------------------------

    reg_t dst = ir_instr->opd1.value.reg;
    reg_t src = ir_instr->opd2.value.reg;

    build_rex   (1,    src > 7,     dst > 7);
    build_modrm (0b11, src & 0b111, dst & 0b111);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t setup_rex_modrm_rm(encoded_instr_t* encoded_instr,
                                 ir_instr_t*      ir_instr)
{
    ASSERT(encoded_instr);
    ASSERT(ir_instr);

    //--------------------------------------------------------------------------

    reg_t dst  = ir_instr->opd1.value.reg;
    int   addr = ir_instr->opd2.value.addr;

    build_rex(1, dst > 7, 0);
    uint8_t mod = 0;
    encoded_instr->info.disp_size = choose_optimal_disp(addr, &mod);
    build_modrm(mod, dst & 0b111, IR_REG_RBP & 0b111);

    encoded_instr->has_disp = true;
    encoded_instr->disp     = addr;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t setup_rex_modrm_mr(encoded_instr_t* encoded_instr,
                                 ir_instr_t*      ir_instr)
{
    ASSERT(encoded_instr);
    ASSERT(ir_instr);

    //--------------------------------------------------------------------------

    int   addr = ir_instr->opd1.value.addr;
    reg_t dst  = ir_instr->opd2.value.reg;

    build_rex(1, dst > 7, 0);
    uint8_t mod = 0;
    encoded_instr->info.disp_size = choose_optimal_disp(addr, &mod);
    build_modrm(mod, dst & 0b111, IR_REG_RBP & 0b111);

    encoded_instr->has_disp = true;
    encoded_instr->disp     = addr;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t setup_rex_modrm_ri(encoded_instr_t* encoded_instr,
                                 ir_instr_t* ir_instr)
{
    ASSERT(encoded_instr);
    ASSERT(ir_instr);

    //--------------------------------------------------------------------------

    reg_t    dst = ir_instr->opd1.value.reg;
    number_t imm = ir_instr->opd2.value.imm;

    build_rex(1, 0, dst > 7);
    build_modrm(0b11, 0, dst & 0b111);

    encoded_instr->has_imm  = true;
    encoded_instr->imm      = imm;
    encoded_instr->imm_size = 4;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t setup_rex_modrm_mi(encoded_instr_t* encoded_instr, ir_instr_t* ir_instr)
{
    ASSERT(encoded_instr);
    ASSERT(ir_instr);

    //--------------------------------------------------------------------------

    int32_t  dst = ir_instr->opd1.value.addr;
    number_t imm = ir_instr->opd2.value.imm;

    build_rex(1, 0, 0);
    uint8_t mod = 0;
    encoded_instr->info.disp_size = choose_optimal_disp(addr, &mod);
    build_modrm(0b10, 0, IR_REG_RBP & 0b111);

    encoded_instr->has_disp = true;
    encoded_instr->disp     = addr;

    encoded_instr->has_imm = true;
    encoded_instr->imm     = imm;
    encoded_instr->imm_size = 4;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

instr_type_t get_instr_type(ir_instr_t* ir_instr)
{
    ASSERT(instr);

    //--------------------------------------------------------------------------

    switch (instr->opd1.type) {
        case IR_OPERAND_REGISTER:
            switch (instr->opd2.type) {
                case IR_OPERAND_REGISTER:  return INSTR_TYPE_REG_REG;
                case IR_OPERAND_MEMORY:    return INSTR_TYPE_REG_MEM;
                case IR_OPERAND_IMMERSIVE: return INSTR_TYPE_REG_IMM;
                default:                   return INSTR_TYPE_UNDEFINED;
            }

        case IR_OPERAND_MEMORY:
            switch (instr->opd2.type) {
                case IR_OPERAND_REGISTER:  return INSTR_TYPE_MEM_REG;
                case IR_OPERAND_IMMERSIVE: return INSTR_TYPE_MEM_IMM;
                default:                   return INSTR_TYPE_UNDEFINED;
            }

        default:
            return INSTR_TYPE_UNDEFINED;
    }

    //--------------------------------------------------------------------------

    return INSTR_TYPE_UNDEFINED;
}

//==============================================================================

lang_status_t setup_rex_modrm_binary_instr(lang_ctx_t* ctx,
                                           ir_instr_t* ir_instr,
                                           encoded_instr_t* encoded_instr,
                                           x86_64_opcode_t opcode)
{
    ASSERT(ctx);
    ASSERT(ir_instr);

    //--------------------------------------------------------------------------

    instr_type_t instr_type = get_instr_type(ir_instr);

    switch(instr_type) {
        case INSTR_TYPE_REG_REG:
            setup_rex_modrm_rr(encoded_instr, ir_instr);
            break;
        case INSTR_TYPE_REG_MEM:
            setup_rex_modrm_rm(encoded_instr, ir_instr);
            break;
        case INSTR_TYPE_MEM_REG:
            setup_rex_modrm_mr(encoded_instr, ir_instr);
            break;
        case INSTR_TYPE_REG_IMM:
            setup_rex_modrm_ri(encoded_instr, ir_instr);
            break;
        case INSTR_TYPE_MEM_IMM:
            setup_rex_modrm_mi(encoded_instr, ir_instr);
            break;
        default:
            return LANG_ERROR;
    }

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_nop(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    *encoded_instr = {.opcode = X86_64_NOP};

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_add(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    setup_rex_modrm_binary_instr(&encoded_instr, ir_instr);
    encoded_instr->opcode = X86_64_ADD_OPCODE;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_sub(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    setup_rex_modrm_binary_instr(&encoded_instr, ir_instr);
    encoded_instr->opcode = X86_64_SUB_OPCODE;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_mul(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_div(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_mov(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    setup_rex_modrm_binary_instr(&encoded_instr, ir_instr);
    encoded_instr->opcode = X86_64_MOV_OPCODE;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_push(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    switch(instr->opd1.type) {
        case IR_OPERAND_REGISTER: {
            uint8_t opc = 0x68; // PUSH imm32
            int32_t imm = (int32_t)instr->opd1.value.imm;
            buffer_append(ctx->byte_code_ctx, &opc, 1);
            buffer_append(ctx->byte_code_ctx, (char*)&imm, 4);
            break;
        }
        case IR_OPERAND_IMMERSIVE: {
            uint8_t opc = 0x68; // PUSH imm32
            int32_t imm = (int32_t)instr->opd1.value.imm;
            buffer_append(ctx->byte_code_ctx, &opc, 1);
            buffer_append(ctx->byte_code_ctx, (char*)&imm, 4);
            break;
        }
        case IR_OPERAND_MEMORY: {
            rex_t rex = { .prefix = 0x4, .w = 1, .r = 0, .x = 0, .b = 0 };
            uint8_t opc = 0xFF; // FF /6 → PUSH r/m
            modrm_t modrm = {
                .mod = 0b10,
                .reg = 6, // /6 → PUSH
                .rm  = 5  // [rbp + disp]
            };

            int32_t disp = instr->opd1.value.addr;

            buffer_append(ctx->byte_code_ctx, (char*)&rex, 1);
            buffer_append(ctx->byte_code_ctx, &opc, 1);
            buffer_append(ctx->byte_code_ctx, (char*)&modrm, 1);
            buffer_append(ctx->byte_code_ctx, (char*)&disp, 4);
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

lang_status_t encode_pop(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    switch(instr->opd1.type) {
        case IR_OPERAND_REGISTER: {
            reg_t reg = instr->opd1.value.reg;

            rex_t rex = get_rex(0, reg); // только расширение B
            rex.w = 0; // для POP RAX, R8 и т.д.

            uint8_t opc = 0x58 + (reg & 0b111);

            if (reg >= 8) {
                buffer_append(ctx->byte_code_ctx, (char*)&rex, 1);
            }

            buffer_append(ctx->byte_code_ctx, &opc, 1);
            break;
        }
        case IR_OPERAND_MEMORY: {
            rex_t rex = { .prefix = 0x4, .w = 1, .r = 0, .x = 0, .b = 0 };
            uint8_t opc = 0x8F; // 8F /0 → POP r/m
            modrm_t modrm = {
                .mod = 0b10,
                .reg = 0, // /0 → POP
                .rm  = 5  // [rbp + disp]
            };

            int32_t disp = instr->opd1.value.addr;

            buffer_append(ctx->byte_code_ctx, (char*)&rex, 1);
            buffer_append(ctx->byte_code_ctx, &opc, 1);
            buffer_append(ctx->byte_code_ctx, (char*)&modrm, 1);
            buffer_append(ctx->byte_code_ctx, (char*)&disp, 4);
        }
        default: {
            return LANG_ERROR;
        }
    }

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_call(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_ret(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    encoded_instr->opcode = X86_64_RET;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_syscall(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    encoded_instr->opcode = (X86_64_SYSCALL_OPCODE2 << 8) + X86_64_SYSCALL_OPCODE1;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_jmp(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_je(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_jne(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_test(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    setup_rex_modrm_binary_instr(ctx, ir_instr, encoded_instr);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_local_label(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_global_label(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

