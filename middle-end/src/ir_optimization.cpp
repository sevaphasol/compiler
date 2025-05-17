#include "lang.h"
#include "ir.h"
#include "ir_list.h"
#include "custom_assert.h"
#include "buffer.h"
#include "graph_dump.h"

//——————————————————————————————————————————————————————————————————————————————

static lang_status_t ir_buf_to_ir_list        (lang_ctx_t* ctx);
static lang_status_t ir_list_to_ir_buf        (lang_ctx_t* ctx);
static lang_status_t remove_push_pop          (lang_ctx_t* ctx);

static lang_status_t replace_push_pop_with_mov(ir_node_t* push_node,
                                               ir_node_t* pop_node);

static lang_status_t remove_trivial_arithmetic(lang_ctx_t* ctx);

static bool ir_instr_is_trivial_arithmetic(ir_instr_t* ir_instr);

//——————————————————————————————————————————————————————————————————————————————

lang_status_t optimize_ir(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    VERIFY(remove_push_pop(ctx),           return LANG_ERROR);
    VERIFY(remove_trivial_arithmetic(ctx), return LANG_ERROR);

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t remove_trivial_arithmetic(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    ir_node_t* cur_node = ctx->ir_list;

    while(cur_node) {
        if (ir_instr_is_trivial_arithmetic(&cur_node->instr)) {
            cut_node(cur_node);
        }

        cur_node = cur_node->next;
    }

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

bool ir_instr_is_trivial_arithmetic(ir_instr_t* ir_instr)
{
    ASSERT(ir_instr);

    ir_opc_t opc     = ir_instr->opc;

    bool opd1_is_num = ir_instr->opd1.type == IR_OPD_IMMEDIATE;
    bool opd2_is_num = ir_instr->opd2.type == IR_OPD_IMMEDIATE;

    int32_t opd1_num = ir_instr->opd1.value.imm;
    int32_t opd2_num = ir_instr->opd2.value.imm;

    bool case1 = (opc == IR_OPC_ADD || opc == IR_OPC_SUB) &
                 (opd1_is_num && opd1_num == 0) ||
                 (opd2_is_num && opd2_num == 0);

    bool case2 = (opc == IR_OPC_IMUL) &&
                 (opd1_is_num && opd1_num == 1) ||
                 (opd2_is_num && opd2_num == 1);

    bool case3 = (opc == IR_OPC_IDIV) &&
                 (opd1_is_num && opd1_num == 1);

    return case1 || case2 || case3;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t remove_push_pop(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    ir_node_t* cur_node = ctx->ir_list;

    while(cur_node && cur_node->next) {
        ir_opc_t cur_opc  = cur_node->instr.opc;
        ir_opc_t next_opc = cur_node->next->instr.opc;

        if (cur_opc == IR_OPC_PUSH && next_opc == IR_OPC_POP) {
            replace_push_pop_with_mov(cur_node, cur_node->next);
        }

        cur_node = cur_node->next;
    }

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t replace_push_pop_with_mov(ir_node_t* push_node,
                                        ir_node_t* pop_node)
{
    ir_opd_t src = push_node->instr.opd1;
    ir_opd_t dst = pop_node->instr.opd1;

    cut_node(push_node);

    ir_instr_t* mov = &pop_node->instr;

    mov->opc  = IR_OPC_MOV;
    mov->opd1 = dst;
    mov->opd2 = src;

    return LANG_SUCCESS;
}
