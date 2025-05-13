//——————————————————————————————————————————————————————————————————————————————

#ifndef _X86_64_CODES_H__
#define _X86_64_CODES_H__

//——————————————————————————————————————————————————————————————————————————————

#define REX_R_UNUSED 0
#define REX_B_UNUSED 0

//——————————————————————————————————————————————————————————————————————————————

enum reg_t {
    REG_RAX = 0,  REG_RCX = 1,  REG_RDX = 2,  REG_RBX = 3,
    REG_RSP = 4,  REG_RBP = 5,  REG_RSI = 6,  REG_RDI = 7,
    REG_R8  = 8,  REG_R9  = 9,  REG_R10 = 10, REG_R11 = 11,
    REG_R12 = 12, REG_R13 = 13, REG_R14 = 14, REG_R15 = 15,
    REG_INV = 16,
};

//——————————————————————————————————————————————————————————————————————————————

const char* const RegNames[] = {
    "rax", "rcx", "rdx", "rbx",
    "rsp", "rbp", "rsi", "rdi",
    "r8",  "r9",  "r10", "r11",
    "r12", "r13", "r14", "r15",
    "invalid",
};

//——————————————————————————————————————————————————————————————————————————————

enum x86_64_opcode_t {
    X86_64_NOP                  = 0x90,

    X86_64_ADD_RR_OPCODE        = 0x01,
    X86_64_ADD_RM_OPCODE        = 0x03,
    X86_64_ADD_MR_OPCODE        = 0x01,
    X86_64_ADD_RI_OPCODE        = 0x81,
    X86_64_ADD_MI_OPCODE        = 0x81,

    X86_64_SUB_RR_OPCODE        = 0x29,
    X86_64_SUB_RM_OPCODE        = 0x2b,
    X86_64_SUB_MR_OPCODE        = 0x29,
    X86_64_SUB_RI_OPCODE        = 0x81,
    X86_64_SUB_MI_OPCODE        = 0x81,

    X86_64_MOV_RR_OPCODE        = 0x89,
    X86_64_MOV_RM_OPCODE        = 0x8b,
    X86_64_MOV_MR_OPCODE        = 0x89,
    X86_64_MOV_RI_OPCODE        = 0xc7,
    X86_64_MOV_MI_OPCODE        = 0xc7,

    X86_64_TEST_RR_OPCODE       = 0x85,
    X86_64_TEST_RM_OPCODE       = 0x85,
    X86_64_TEST_MR_OPCODE       = 0x85,
    X86_64_TEST_RI_OPCODE       = 0xf7,
    X86_64_TEST_MI_OPCODE       = 0xf7,

    X86_64_PUSH_R_OPCODE       = 0x50,
    X86_64_PUSH_I_OPCODE       = 0x68,
    X86_64_PUSH_M_OPCODE       = 0xff,

    X86_64_POP_R_OPCODE        = 0x58,
    X86_64_POP_M_OPCODE        = 0x8f,

    X86_64_CALL_REL32_OPCODE = 0xE8,

    X86_64_RET               = 0xC3,

    X86_64_JMP_REL32_OPCODE  = 0xE9,

    X86_64_JE_REL32_OPCODE1  = 0x0F,
    X86_64_JE_REL32_OPCODE2  = 0x84,

    X86_64_JNE_REL32_OPCODE1 = 0x0F,
    X86_64_JNE_REL32_OPCODE2 = 0x85,

    X86_64_SYSCALL_OPCODE1   = 0x0F,
    X86_64_SYSCALL_OPCODE2   = 0x05,
};

//——————————————————————————————————————————————————————————————————————————————

enum x86_64_modrm_reg_t {
    X86_64_PUSH_M_MODRM_REG = 6,
    X86_64_POP_M_MODRM_REG  = 0,
};

//——————————————————————————————————————————————————————————————————————————————

enum x86_64_modrm_mod {
    X86_64_MOD_M_NO_DISP = 0b00,
    X86_64_MOD_M_DISP8   = 0b01,
    X86_64_MOD_M_DISP32  = 0b10,
    X86_64_MOD_RR        = 0b11,
    X86_64_MOD_RI        = 0b11,
    X86_64_MOD_MI        = 0b11,
};

//——————————————————————————————————————————————————————————————————————————————

#endif // _X86_64_CODES_H__

//——————————————————————————————————————————————————————————————————————————————
