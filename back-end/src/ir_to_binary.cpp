#include <string.h>

#include "ir.h"
#include "custom_assert.h"
#include "x86_64_codes.h"
#include "x86_64_opc_tables.h"
#include "buffer.h"
#include "fixup_table.h"

//——————————————————————————————————————————————————————————————————————————————

lang_status_t encode_instr(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    return IrOpEncodeFuncsTable[ir_instr->opc](ctx, ir_instr, bin_instr);
}

//==============================================================================

lang_status_t emit_bin_instr(buffer_t* bin_buf, const bin_instr_t* instr)
{
    ASSERT(bin_buf);
    ASSERT(instr);

    //--------------------------------------------------------------------------

    if (instr->info.has_rex)
        buf_write(bin_buf, &instr->rex, sizeof(instr->rex));

    buf_write(bin_buf, &instr->opc, instr->info.opcode_size);

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

lang_status_t ir_to_binary(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    //--------------------------------------------------------------------------

    size_t size = ctx->ir_buf.size / sizeof(ir_instr_t);
    ir_instr_t* ir_buf = (ir_instr_t*) ctx->ir_buf.data;

    for (size_t i = 0; i < size; i++) {
        ir_instr_t ir_instr = ir_buf[i];
        bin_instr_t bin_instr = {};
        bin_instr.info.opcode_size = 1;
        encode_instr(ctx, &ir_instr, &bin_instr);
        if (ir_instr.opc != IR_OPC_GLOBAL_LABEL &&
            ir_instr.opc != IR_OPC_LOCAL_LABEL) {
            emit_bin_instr(&ctx->bin_buf, &bin_instr);
        }
    }

    fixup_table_apply(&ctx->fixups, &ctx->label_table, &ctx->ir_buf);

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

modrm_t build_modrm(uint8_t mod, uint8_t reg, uint8_t rm)
{
    return (modrm_t) {
        .rm  = rm,
        .reg = reg,
        .mod = mod
    };
}

//==============================================================================

rex_t build_rex(uint8_t w, uint8_t r, uint8_t b)
{
    return (rex_t) {
        .b      = b,
        .x      = 0, // SIB is never used
        .r      = r,
        .w      = w,
        .prefix = 0b0100 // fixed constant
    };
}

//==============================================================================

lang_status_t build_rex_modrm_rr(bin_instr_t* bin_instr, ir_instr_t* ir_instr, uint8_t modrm_reg)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    //--------------------------------------------------------------------------

    reg_t dst = ir_instr->opd1.value.reg;
    reg_t src = ir_instr->opd2.value.reg;

    bin_instr->rex   = build_rex   (1,    src > 7,     dst > 7);
    bin_instr->modrm = build_modrm (0b11, src & 0b111, dst & 0b111);

    bin_instr->info.has_rex   = true;
    bin_instr->info.has_modrm = true;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t build_rex_modrm_rm(bin_instr_t* bin_instr, ir_instr_t* ir_instr, uint8_t modrm_reg)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    //--------------------------------------------------------------------------

    reg_t dst    = ir_instr->opd1.value.reg;
    int   offset = ir_instr->opd2.value.offset;

    uint8_t mod = 0;
    bin_instr->info.disp_size = choose_optimal_disp(offset, &mod);

    bin_instr->rex   = build_rex  (1,   dst > 7,     0);
    bin_instr->modrm = build_modrm(mod, dst & 0b111, REG_RBP & 0b111);

    bin_instr->info.has_rex   = true;
    bin_instr->info.has_modrm = true;

    bin_instr->info.has_disp = true;
    bin_instr->disp     = offset;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t build_rex_modrm_mr(bin_instr_t* bin_instr, ir_instr_t* ir_instr, uint8_t modrm_reg)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    //--------------------------------------------------------------------------

    int   offset = ir_instr->opd1.value.offset;
    reg_t dst    = ir_instr->opd2.value.reg;

    uint8_t mod = 0;
    bin_instr->info.disp_size = choose_optimal_disp(offset, &mod);

    bin_instr->rex   = build_rex  (1,   dst > 7,     0);
    bin_instr->modrm = build_modrm(mod, dst & 0b111, REG_RBP & 0b111);

    bin_instr->info.has_rex   = true;
    bin_instr->info.has_modrm = true;

    bin_instr->info.has_disp = true;
    bin_instr->disp     = offset;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t build_rex_modrm_ri(bin_instr_t* bin_instr, ir_instr_t* ir_instr, uint8_t modrm_reg)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    //--------------------------------------------------------------------------

    reg_t    dst = ir_instr->opd1.value.reg;
    number_t imm = ir_instr->opd2.value.imm;

    bin_instr->rex   = build_rex  (1,    0,         dst > 7);
    bin_instr->modrm = build_modrm(0b11, modrm_reg, dst & 0b111);

    bin_instr->info.has_rex   = true;
    bin_instr->info.has_modrm = true;

    bin_instr->info.has_imm  = true;
    bin_instr->imm      = imm;
    bin_instr->info.imm_size = 4;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t build_rex_modrm_mi(bin_instr_t* bin_instr, ir_instr_t* ir_instr, uint8_t modrm_reg)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    //--------------------------------------------------------------------------

    int32_t  offset = ir_instr->opd1.value.offset;
    number_t imm    = ir_instr->opd2.value.imm;

    uint8_t mod = 0;
    bin_instr->info.disp_size = choose_optimal_disp(offset, &mod);

    bin_instr->rex   = build_rex  (1,    0,         0);
    bin_instr->modrm = build_modrm(0b10, modrm_reg, REG_RBP & 0b111);

    bin_instr->info.has_rex   = true;
    bin_instr->info.has_modrm = true;

    bin_instr->info.has_disp = true;
    bin_instr->disp     = offset;

    bin_instr->info.has_imm = true;
    bin_instr->imm     = imm;
    bin_instr->info.imm_size = 4;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

ir_instr_type_t get_ir_instr_type(ir_instr_t* ir_instr)
{
    ASSERT(ir_instr);

    //--------------------------------------------------------------------------

    switch (ir_instr->opd1.type) {
        case IR_OPD_REGISTER:
            switch (ir_instr->opd2.type) {
                case IR_OPD_REGISTER:  return IR_INSTR_TYPE_REG_REG;
                case IR_OPD_MEMORY:    return IR_INSTR_TYPE_REG_MEM;
                case IR_OPD_IMMEDIATE: return IR_INSTR_TYPE_REG_IMM;
                default:               return IR_INSTR_TYPE_UNDEFINED;
            }

        case IR_OPD_MEMORY:
            switch (ir_instr->opd2.type) {
                case IR_OPD_REGISTER:  return IR_INSTR_TYPE_MEM_REG;
                case IR_OPD_IMMEDIATE: return IR_INSTR_TYPE_MEM_IMM;
                default:               return IR_INSTR_TYPE_UNDEFINED;
            }

        default:
            return IR_INSTR_TYPE_UNDEFINED;
    }

    //--------------------------------------------------------------------------

    return IR_INSTR_TYPE_UNDEFINED;
}

//==============================================================================

lang_status_t set_opc_and_modrm_reg(ir_opc_t ir_opc, ir_instr_type_t ir_instr_type,
                                    uint16_t* bin_opc, uint8_t* modrm_reg)
{
    ASSERT(bin_opc);
    ASSERT(modrm_reg);

    switch(ir_opc) {
        case IR_OPC_ADD:
            *bin_opc   = AddOpcInfo[ir_instr_type].opc;
            *modrm_reg = AddOpcInfo[ir_instr_type].modrm_reg;
            break;
        case IR_OPC_SUB:
            *bin_opc   = SubOpcInfo[ir_instr_type].opc;
            *modrm_reg = SubOpcInfo[ir_instr_type].modrm_reg;
            break;
        case IR_OPC_MOV:
            *bin_opc   = MovOpcInfo[ir_instr_type].opc;
            *modrm_reg = MovOpcInfo[ir_instr_type].modrm_reg;
            break;
        case IR_OPC_TEST:
            *bin_opc   = TestOpcInfo[ir_instr_type].opc;
            *modrm_reg = TestOpcInfo[ir_instr_type].modrm_reg;
            break;
        default:
            return LANG_ERROR;
    }

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_two_operand_instr(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    //--------------------------------------------------------------------------

    ir_instr_type_t ir_instr_type   = get_ir_instr_type(ir_instr);
    instr_opc_info_t* opc_info = nullptr;

    uint8_t modrm_reg = 0;

    set_opc_and_modrm_reg(ir_instr->opc, ir_instr_type, &bin_instr->opc, &modrm_reg);

    //--------------------------------------------------------------------------

    switch(ir_instr_type) {
        case IR_INSTR_TYPE_REG_REG:
            build_rex_modrm_rr(bin_instr, ir_instr, modrm_reg);
            break;
        case IR_INSTR_TYPE_REG_MEM:
            build_rex_modrm_rm(bin_instr, ir_instr, modrm_reg);
            break;
        case IR_INSTR_TYPE_MEM_REG:
            build_rex_modrm_mr(bin_instr, ir_instr, modrm_reg);
            break;
        case IR_INSTR_TYPE_REG_IMM:
            build_rex_modrm_ri(bin_instr, ir_instr, modrm_reg);
            break;
        case IR_INSTR_TYPE_MEM_IMM:
            build_rex_modrm_mi(bin_instr, ir_instr, modrm_reg);
            break;
        default:
            return LANG_ERROR;
    }

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_mov(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------
    ir_instr_type_t ir_instr_type = get_ir_instr_type(ir_instr);
    uint16_t opc;
    uint8_t modrm_reg;

    set_opc_and_modrm_reg(ir_instr->opc, ir_instr_type, &opc, &modrm_reg);

    bin_instr->opc = opc;
    //--------------------------------------------------------------------------

    switch (ir_instr_type) {
        case IR_INSTR_TYPE_REG_REG:
            build_rex_modrm_rr(bin_instr, ir_instr, modrm_reg);
            break;

        case IR_INSTR_TYPE_REG_MEM:
            build_rex_modrm_rm(bin_instr, ir_instr, modrm_reg);
            break;

        case IR_INSTR_TYPE_MEM_REG:
            build_rex_modrm_mr(bin_instr, ir_instr, modrm_reg);
            break;

        case IR_INSTR_TYPE_REG_IMM: {
            reg_t dst = ir_instr->opd1.value.reg;

            if (dst <= REG_RDI) {
                bin_instr->opc = 0xB8 + (dst & 0b111);
                bin_instr->rex = build_rex(1, 0, dst > 7);
                bin_instr->info.has_rex = true;

                bin_instr->info.has_imm = true;
                bin_instr->info.imm_size = 8;
                bin_instr->imm = ir_instr->opd2.value.imm;
            } else {
                bin_instr->rex = build_rex(1, 0, 1);
                bin_instr->info.has_rex = true;
                bin_instr->opc = 0xC7;
                bin_instr->modrm = build_modrm(0b11, 0, dst & 0b111);
                bin_instr->info.has_modrm = true;

                bin_instr->info.has_imm = true;
                bin_instr->info.imm_size = 8;
                bin_instr->imm = ir_instr->opd2.value.imm;
            }
            break;
        }

        case IR_INSTR_TYPE_MEM_IMM:
            build_rex_modrm_mi(bin_instr, ir_instr, modrm_reg);
            break;

        default:
            return LANG_ERROR;
    }

    //--------------------------------------------------------------------------
    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_nop(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------

    bin_instr->opc = X86_64_NOP;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_add(lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

lang_status_t encode_sub(lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_mul(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_div(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_push(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------

    switch(ir_instr->opd1.type) {
        case IR_OPD_REGISTER: {
            uint8_t opc = X86_64_PUSH_R_OPCODE;
            reg_t   reg = ir_instr->opd1.value.reg;

            if (reg >= REG_R8) {
                bin_instr->rex = build_rex(1, 0, 1);
                bin_instr->info.has_rex = true;
            }

            bin_instr->opc = opc + (uint8_t) reg;

            break;
        }
        case IR_OPD_IMMEDIATE: {
            bin_instr->opc = X86_64_PUSH_I_OPCODE;
            bin_instr->info.has_imm = true;
            bin_instr->info.imm_size = 4;
            bin_instr->imm = ir_instr->opd1.value.imm;
            break;
        }
        case IR_OPD_MEMORY: {
            bin_instr->opc = X86_64_PUSH_M_OPCODE;
            build_rex_modrm_mr(bin_instr, ir_instr, 0);
            bin_instr->modrm.reg = 6; // /6 for PUSH
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

lang_status_t encode_pop(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------

    switch(ir_instr->opd1.type) {
        case IR_OPD_REGISTER: {
            uint8_t opc = X86_64_POP_R_OPCODE;
            reg_t   reg = ir_instr->opd1.value.reg;

            if (reg >= REG_R8) {
                bin_instr->rex = build_rex(1, 0, 1);
                bin_instr->info.has_rex = true;
            }

            bin_instr->opc = opc + (uint8_t) reg;

            break;
        }
        case IR_OPD_MEMORY: {
           bin_instr->opc = X86_64_POP_M_OPCODE;
            build_rex_modrm_mr(bin_instr, ir_instr, 0);
            bin_instr->modrm.reg = 0; // /0 for POP
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

lang_status_t encode_call(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------

    ASSERT(ir_instr->opd1.type == IR_OPD_GLOBAL_LABEL);

    const char* name = ir_instr->opd1.value.global_label_name;

    bin_instr->opc = X86_64_CALL_REL32_OPCODE;
    bin_instr->info.has_imm = true;
    bin_instr->info.imm_size = 4;
    bin_instr->imm = 0;

    add_fixup(&ctx->fixups, name, 0, ctx->ir_buf.size + 1);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_ret(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------

    bin_instr->opc = X86_64_RET;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_syscall(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------

    bin_instr->opc = (X86_64_SYSCALL_OPCODE2 << 8) + X86_64_SYSCALL_OPCODE1;
    bin_instr->info.opcode_size = 2;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_jumps(lang_ctx_t*  ctx,
                           ir_instr_t*  ir_instr,
                           bin_instr_t* bin_instr,
                           uint16_t     opc)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------

    ASSERT(ir_instr->opd1.type == IR_OPD_LOCAL_LABEL);

    size_t label_num = ir_instr->opd1.value.local_label_number;

    bin_instr->opc = opc;
    bin_instr->info.has_imm = true;
    bin_instr->info.imm_size = 4;
    bin_instr->imm = 0;

    add_fixup(&ctx->fixups, NULL, label_num, ctx->bin_buf.size + 1);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_jmp(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    return encode_jumps(ctx, ir_instr, bin_instr, X86_64_JMP_REL32_OPCODE);
}

//==============================================================================

lang_status_t encode_je(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(bin_instr);

    uint16_t opc = (X86_64_JE_REL32_OPCODE2 << 8) + X86_64_JE_REL32_OPCODE1;
    bin_instr->info.opcode_size = 2;

    return encode_jumps(ctx, ir_instr, bin_instr, opc);
}

//==============================================================================

lang_status_t encode_jne(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(bin_instr);

    uint16_t opc = (X86_64_JNE_REL32_OPCODE2 << 8) + X86_64_JNE_REL32_OPCODE1;
    bin_instr->info.opcode_size = 2;

    return encode_jumps(ctx, ir_instr, bin_instr, opc);

}

//==============================================================================

lang_status_t encode_test(lang_ctx_t*  ctx, ir_instr_t*  ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_local_label(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------

    size_t label_num = ir_instr->opd1.value.local_label_number;
    size_t curr_addr = ctx->ir_buf.size;
    label_table_add_local(&ctx->label_table, label_num, curr_addr);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t encode_global_label(lang_ctx_t* ctx, ir_instr_t* ir_instr, bin_instr_t* bin_instr)
{
    ASSERT(ctx);
    ASSERT(ir_instr);
    ASSERT(bin_instr);

    //--------------------------------------------------------------------------

    const char* name = ir_instr->opd1.value.global_label_name;
    size_t curr_addr = ctx->ir_buf.size;
    label_table_add_global(&ctx->label_table, name, curr_addr);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

