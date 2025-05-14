#include "encode_utils.h"
#include "custom_assert.h"

//——————————————————————————————————————————————————————————————————————————————

bool reg_expand(reg_t reg)
{
    return reg >= REG_R8;
}

//——————————————————————————————————————————————————————————————————————————————

reg_t trim_reg(reg_t reg)
{
    return (reg_t) (reg & 0b111);
}

//——————————————————————————————————————————————————————————————————————————————

void set_mod_and_disp_size(int32_t disp, uint8_t* mod, uint8_t* disp_size)
{
    ASSERT(mod);
    ASSERT(disp_size);

    if (disp == 0) {
        *mod       = X86_64_MOD_M_NO_DISP;
        *disp_size = 0;
    } else if (disp >= -128 && disp <= 127) {
        *mod       = X86_64_MOD_M_DISP8;
        *disp_size = 1;
    } else {
        *mod       = X86_64_MOD_M_DISP32;
        *disp_size = 4;
    }
}

//——————————————————————————————————————————————————————————————————————————————

modrm_t build_modrm(uint8_t mod, uint8_t reg, uint8_t rm)
{
    return (modrm_t) {
        .rm  = rm,
        .reg = reg,
        .mod = mod
    };
}

//——————————————————————————————————————————————————————————————————————————————

rex_t build_rex(uint8_t r, uint8_t b)
{
    return (rex_t) {
        .b      = b,
        .x      = 0, // SIB is never used in this implementation
        .r      = r,
        .w      = 1, // Always 64 bit operands in this implementation
        .prefix = 0b0100 // fixed constant
    };
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t build_rex_rr(bin_instr_t* bin_instr, ir_instr_t* ir_instr)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    reg_t dst = ir_instr->opd1.value.reg;
    reg_t src = ir_instr->opd2.value.reg;

    bin_instr->rex = build_rex(REG_RAX, reg_expand(dst));

    bin_instr->info.has_rex = true;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t build_modrm_rr(bin_instr_t* bin_instr, ir_instr_t*  ir_instr)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    reg_t dst = ir_instr->opd1.value.reg;
    reg_t src = ir_instr->opd2.value.reg;

    bin_instr->modrm = build_modrm(X86_64_MOD_RR, trim_reg(src),
                                                  trim_reg(dst));

    bin_instr->info.has_modrm = true;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t build_rex_rm(bin_instr_t* bin_instr, ir_instr_t*  ir_instr)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    reg_t dst = ir_instr->opd1.value.reg;

    bin_instr->rex = build_rex(reg_expand(dst),
                               reg_expand(REG_RBP));

    bin_instr->info.has_rex = true;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t build_modrm_rm(bin_instr_t* bin_instr, ir_instr_t* ir_instr)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    reg_t   dst       = ir_instr->opd1.value.reg;
    int32_t disp      = ir_instr->opd2.value.offset;
    uint8_t mod       = 0;
    uint8_t disp_size = 0;

    set_mod_and_disp_size(disp, &mod, &disp_size);

    bin_instr->modrm = build_modrm(mod, trim_reg(dst),
                                        trim_reg(REG_RBP));

    bin_instr->info.has_modrm = true;
    bin_instr->info.has_disp  = true;
    bin_instr->disp           = disp;
    bin_instr->info.disp_size = disp_size;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t build_rex_mr(bin_instr_t* bin_instr, ir_instr_t* ir_instr)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    reg_t dst = ir_instr->opd2.value.reg;

    bin_instr->rex = build_rex(reg_expand(dst),
                               reg_expand(REG_RBP));

    bin_instr->info.has_rex = true;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t build_modrm_mr(bin_instr_t* bin_instr, ir_instr_t* ir_instr)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    reg_t dst         = ir_instr->opd2.value.reg;
    int32_t disp      = ir_instr->opd1.value.offset;
    uint8_t mod       = 0;
    uint8_t disp_size = 0;

    set_mod_and_disp_size(disp, &mod, &disp_size);

    bin_instr->modrm = build_modrm(mod, trim_reg(dst),
                                        trim_reg(REG_RBP));

    bin_instr->info.has_modrm = true;
    bin_instr->info.has_disp  = true;
    bin_instr->disp           = disp;
    bin_instr->info.disp_size = disp_size;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t build_rex_ri(bin_instr_t* bin_instr, ir_instr_t* ir_instr)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    reg_t dst = ir_instr->opd1.value.reg;

    bin_instr->rex = build_rex(REX_R_UNUSED, reg_expand(dst));

    bin_instr->info.has_rex = true;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t build_modrm_and_imm_ri(bin_instr_t* bin_instr,
                                     ir_instr_t*  ir_instr,
                                     uint8_t      modrm_reg)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    reg_t dst    = ir_instr->opd1.value.reg;
    number_t imm = ir_instr->opd2.value.imm;

    bin_instr->modrm = build_modrm(X86_64_MOD_RI, modrm_reg, trim_reg(dst));

    bin_instr->info.has_modrm   = true;
    bin_instr->info.has_imm     = true;
    bin_instr->info.imm_size    = 4;
    bin_instr->imm              = imm;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t build_rex_mi(bin_instr_t* bin_instr, ir_instr_t* ir_instr)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    bin_instr->rex = build_rex(REX_R_UNUSED, REX_B_UNUSED);
    bin_instr->info.has_rex = true;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t build_modrm_and_imm_mi(bin_instr_t* bin_instr,
                                     ir_instr_t*  ir_instr,
                                     uint8_t      modrm_reg)
{
    ASSERT(bin_instr);
    ASSERT(ir_instr);

    int32_t disp = ir_instr->opd1.value.offset;
    number_t imm = ir_instr->opd2.value.imm;

    uint8_t mod       = 0;
    uint8_t disp_size = 0;

    set_mod_and_disp_size(disp, &mod, &disp_size);

    bin_instr->modrm = build_modrm(mod, modrm_reg, trim_reg(REG_RBP));

    bin_instr->info.has_modrm = true;

    if (disp_size) {
        bin_instr->info.has_disp = true;
        bin_instr->disp = disp;
        bin_instr->info.disp_size = disp_size;
    } else {
        bin_instr->info.has_disp = false;
    }

    bin_instr->info.has_imm     = true;
    bin_instr->info.imm_size    = 4;
    bin_instr->imm              = imm;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

ir_instr_type_t get_ir_instr_type(ir_instr_t* ir_instr)
{
    ASSERT(ir_instr);

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

    return IR_INSTR_TYPE_UNDEFINED;
}

//——————————————————————————————————————————————————————————————————————————————

x86_64_opcode_t get_long_opcode(uint16_t low_part, uint16_t high_part)
{
    return (x86_64_opcode_t) ((high_part << 8) | low_part);
}

//——————————————————————————————————————————————————————————————————————————————
