#ifndef _X86_64_OPCODES_H__
#define _X86_64_OPCODES_H__

enum x86_64_opcode_t
{
    X86_64_NOP               = 0x90,
    X86_64_ADD_OPCODE        = 0x01,
    X86_64_SUB_OPCODE        = 0x29,
    X86_64_MOV_OPCODE        = 0x89,
    X86_64_PUSH_OPCODE       = 0x50,
    X86_64_POP_OPCODE        = 0x58,
    X86_64_CALL_REL32_OPCODE = 0xE8,
    X86_64_RET               = 0xC3,
    X86_64_TEST_OPCODE       = 0x85,
    X86_64_JMP_REL32_OPCODE  = 0xE9,
    X86_64_JE_REL32_OPCODE1  = 0x84,
    X86_64_JE_REL32_OPCODE2  = 0x0F,
    X86_64_JNE_REL32_OPCODE1 = 0x85,
    X86_64_JNE_REL32_OPCODE2 = 0x0F,
    X86_64_SYSCALL_OPCODE1   = 0x0F,
    X86_64_SYSCALL_OPCODE2   = 0x05,
};

#endif // _X86_64_OPCODES_H__
