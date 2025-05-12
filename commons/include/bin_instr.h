//——————————————————————————————————————————————————————————————————————————————

#ifndef _ENCODER_H__
#define _ENCODER_H__

//——————————————————————————————————————————————————————————————————————————————

struct rex_t {
    uint8_t b      : 1;
    uint8_t x      : 1;
    uint8_t r      : 1;
    uint8_t w      : 1;
    uint8_t prefix : 4;
};

//——————————————————————————————————————————————————————————————————————————————

struct modrm_t {
    uint8_t rm  : 3;
    uint8_t reg : 3;
    uint8_t mod : 2;
};

//——————————————————————————————————————————————————————————————————————————————

struct bin_instr_info_t {
    bool    has_rex;
    bool    has_modrm;
    bool    has_disp;
    bool    has_imm;
    uint8_t disp_size;
    uint8_t imm_size;
    uint8_t opcode_size;
};

//——————————————————————————————————————————————————————————————————————————————

struct bin_instr_t {
    rex_t            rex;
    uint16_t         opcode;
    modrm_t          modrm;
    int32_t          disp;
    int64_t          imm;
    bin_instr_info_t info;
};

//——————————————————————————————————————————————————————————————————————————————

#endif // _ENCODER_H__

//——————————————————————————————————————————————————————————————————————————————
