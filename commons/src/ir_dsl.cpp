#include "ir.h"
#include "custom_assert.h"

//—————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————

ir_opd_t opd_reg(reg_t reg)
{
    return (ir_opd_t) {.type  = IR_OPD_REGISTER, .value = {.reg = reg}};
}

//——————————————————————————————————————————————————————————————————————————————

ir_opd_t opd_mem(int32_t offset)
{
    return (ir_opd_t) {.type  = IR_OPD_MEMORY, .value = {.offset = offset}};
}

//——————————————————————————————————————————————————————————————————————————————

ir_opd_t opd_imm(float imm)
{
    return (ir_opd_t) {.type  = IR_OPD_IMMEDIATE, .value = {.imm = imm}};
}

//——————————————————————————————————————————————————————————————————————————————

ir_opd_t opd_global_label(const char* global_label_name)
{
    return (ir_opd_t) {.type  = IR_OPD_GLOBAL_LABEL,
                       .value = {.global_label_name = global_label_name}};
}

//——————————————————————————————————————————————————————————————————————————————

ir_opd_t operand_local_label(uint64_t local_label_number)
{
    return (ir_opd_t) {.type  = IR_OPD_LOCAL_LABEL,
                       .value = {.local_label_number = local_label_number}};
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_nop(buffer_t* ir_buf)
{
    return ir_emit_instr(ir_buf, {IR_OPC_NOP, {}, {}});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_binary_operation(buffer_t* ir_buf, ir_opc_t opc, ir_opd_t opd1, ir_opd_t opd2)
{
    return ir_emit_instr(ir_buf, {opc, opd1, opd2});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_add(buffer_t* ir_buf, ir_opd_t opd1, ir_opd_t opd2)
{
    return ir_emit_instr(ir_buf, {IR_OPC_ADD, opd1, opd2});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_sub(buffer_t* ir_buf, ir_opd_t opd1, ir_opd_t opd2)
{
    return ir_emit_instr(ir_buf, {IR_OPC_SUB, opd1, opd2});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_mul(buffer_t* ir_buf, ir_opd_t opd1, ir_opd_t opd2)
{
    return ir_emit_instr(ir_buf, {IR_OPC_MUL, opd1, opd2});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_div(buffer_t* ir_buf, ir_opd_t opd1, ir_opd_t opd2)
{
    return ir_emit_instr(ir_buf, {IR_OPC_DIV, opd1, opd2});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_mov(buffer_t* ir_buf, ir_opd_t dst, ir_opd_t src)
{
    return ir_emit_instr(ir_buf, {IR_OPC_MOV, dst, src});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_push(buffer_t* ir_buf, ir_opd_t src)
{
    return ir_emit_instr(ir_buf, {IR_OPC_PUSH, src, {}});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_pop(buffer_t* ir_buf, ir_opd_t dst)
{
    return ir_emit_instr(ir_buf, {IR_OPC_POP, dst, {}});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_call(buffer_t* ir_buf, const char* name)
{
    return ir_emit_instr(ir_buf, {IR_OPC_CALL, operand_global_label(name), {}});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_ret(buffer_t* ir_buf)
{
    return ir_emit_instr(ir_buf, {IR_OPC_RET, {}, {}});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_syscall(buffer_t* ir_buf)
{
    return ir_emit_instr(ir_buf, {IR_OPC_SYSCALL, {}, {}});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_global_label(buffer_t* ir_buf, const char* name)
{
    return ir_emit_instr(ir_buf, {IR_OPC_GLOBAL_LABEL, operand_global_label(name), {}});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_local_label(buffer_t* ir_buf, int64_t label_number)
{
    return ir_emit_instr(ir_buf, {IR_OPC_LOCAL_LABEL, operand_local_label(label_number), {}});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_jmp(buffer_t* ir_buf, int64_t label_number)
{
    return ir_emit_instr(ir_buf, {IR_OPC_JMP, operand_local_label(label_number), {}});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_je(buffer_t* ir_buf, int64_t label_number)
{
    return ir_emit_instr(ir_buf, {IR_OPC_JE, operand_local_label(label_number), {}});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_jne(buffer_t* ir_buf, int64_t label_number)
{
    return ir_emit_instr(ir_buf, {IR_OPC_JNE, operand_local_label(label_number), {}});
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t emit_test(buffer_t* ir_buf, ir_opd_t opd1, ir_opd_t opd2)
{
    return ir_emit_instr(ir_buf, {IR_OPC_TEST, opd1, opd2});
}

//——————————————————————————————————————————————————————————————————————————————
