#include <string.h>

#include "IR.h"
#include "custom_assert.h"
#include "x86_64_opcodes.h"

//——————————————————————————————————————————————————————————————————————————————

lang_status_t byte_buffer_ctor(byte_code_ctx_t* ctx, size_t init_capacity)
{
    ASSERT(ctx);
    ASSERT(init_capacity);

    //--------------------------------------------------------------------------

    ctx->buffer = (char*) calloc(init_capacity, sizeof(char));
    if (!ctx->buffer) {
        return LANG_STD_ALLOCATE_ERROR;
    }

    ctx->buffer_capacity = init_capacity;
    ctx->buffer_size     = 0;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t byte_buferr_dtor(byte_code_ctx_t* ctx)
{
    ASSERT(ctx);

    if (!ctx->buffer) {
        return LANG_ERROR;
    }

    free(ctx->buffer);

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t buffer_append(byte_code_ctx_t* ctx, char* byte_code, size_t size)
{
    ASSERT(ctx);
    ASSERT(byte_code);

    //--------------------------------------------------------------------------

    if (ctx->buffer_size >= ctx->buffer_capacity) {
        ctx->buffer_capacity *= 2;
        ctx->buffer = (char*) realloc(ctx->buffer, ctx->buffer_capacity * sizeof(char));
    }

    if (!ctx->buffer) {
        return LANG_STD_ALLOCATE_ERROR;
    }

    //--------------------------------------------------------------------------

    memcpy(ctx->buffer + ctx->buffer_size, byte_code, size);
    ctx->buffer_size += size;

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
        ir_instr_t instr = ctx->ir_ctx->buffer[i];
        encoded_instr_t encoded_instr = {};
        encode_instr(ctx, &instr, &encoded_instr);
        emit_encoded_instr(ctx, &encoded_instr);
    }

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_instr(lang_ctx_t* ctx, ir_instr_t* ir_instr, encoded_instr_t* encoded_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(encoded_instr);

    return (*OpcodesTable[instr->op].encode_func)(ctx, instr, encoded_instr);
}

//==============================================================================

lang_status_t emit_encoded_instr(lang_ctx_t* ctx, const encoded_instr_t* instr)
{
    ASSERT(ctx);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    byte_code_ctx_t* buffer = ctx->byte_code_ctx->buffer;

    //--------------------------------------------------------------------------

    if (instr->info.has_rex) {
        buffer_append(buffer, (char*) &instr->rex, sizeof(instr->rex));
    }

    buffer_append(buffer, (char*) &instr->opcode, instr->info.opcode_size);

    if (instr->info.has_modrm) {
        buffer_append(buffer, (char*) &instr->modrm, sizeof(instr->modrm));
    }

    if (instr->info.has_disp) {
        buffer_append(buffer, (char*) &instr->disp, instr->info.disp_size);
    }

    if (instr->info.has_imm) {
        buffer_append(buffer, (char*) &instr->imm, instr->info.imm_size);
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

lang_status_t setup_rex_modrm_rr(encoded_instr_t* encoded_instr,
                                 ir_instr_t*      ir_instr)
{
    ASSERT(encoded_instr);
    ASSERT(ir_instr);

    //--------------------------------------------------------------------------

    reg_t dst = ir_instr->opd1.value.reg;
    reg_t src = ir_instr->opd2.value.reg;

    encoded_instr->rex = {
        .prefix = 0b0100,  // fixed constant
        .w      = 1,       // always 64-bit operands
        .r      = src > 7, // extension of reg
        .x      = 0,       // SIB is never used
        .b      = dst > 7  // extension of rm
    };

    encoded_instr->modrm = {
        .mod = 0b11,        // register → register mode
        .reg = src & 0b111,
        .rm  = dst & 0b111
    };

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

    encoded_instr->rex = {
        .prefix = 0b0100,
        .w      = 1,
        .r      = dst > 7,
        .x      = 0,
        .b      = 0, // base_reg is always RBP — no need for extension
    };

    uint8_t mod = 0;

    encoded_instr->info.disp_size = choose_optimal_disp(addr, &mod);

    encoded_instr->modrm = {
        .mod = mod,
        .reg = dst        & 0b111,
        .rm  = IR_REG_RBP & 0b111,
    };

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

    encoded_instr->rex = {
        .prefix = 0b0100,
        .w      = 1,
        .r      = dst > 7,
        .x      = 0,
        .b      = 0, // base_reg is always RBP — no need for extension
    };

    uint8_t mod = 0;

    encoded_instr->info.disp_size = choose_optimal_disp(addr, &mod);

    encoded_instr->modrm = {
        .mod = mod,
        .reg = dst & 0b111,
        .rm  = IR_REG_RBP & 0b111,
    };

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

    encoded_instr->rex = {
        .prefix = 0b0100,
        .w      = 1,
        .r      = 0, // No source register
        .x      = 0,
        .b      = dst > 7, //
    };

    encoded_instr->modrm = {
        .mod = 0b11, // immediate → register mode
        .reg = 0,    // will be inited by caller
        .rm  = dst & 0b111,
    };

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

    encoded_instr->rex = {
        .prefix = 0b0100,
        .w      = 1,
        .r      = 0, // No registers
        .x      = 0,
        .b      = 0, // base_reg = RBP
    };

    uint8_t mod = 0;

    encoded_instr->info.disp_size = choose_optimal_disp(addr, &mod);

    encoded_instr->modrm = {
        .mod = 0b10,
        .reg = 0, // will be inited by caller
        .rm  = IR_REG_RBP & 0b111,
    };

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

