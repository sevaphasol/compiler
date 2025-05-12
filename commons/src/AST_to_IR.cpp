#include <stack>

#include "lang.h"
#include "custom_assert.h"
#include "ir.h"

#define _DSL_DEFINE_
#include "dsl.h"

//==============================================================================

lang_status_t build_ir(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    //--------------------------------------------------------------------------

    emit_global_label(ctx, "_start");
    emit_call        (ctx, "main");

    operand_t return_val = operand_register(REG_RAX);
    operand_t exit_code  = operand_immersive(60);

    emit_mov(ctx, operand_register(IR_REG_RAX),
                  operand_immersive(60));
    emit_syscall(ctx);

    //--------------------------------------------------------------------------

    lang_status_t status = node_to_IR(ctx, ctx->tree);

    //--------------------------------------------------------------------------

    return status;
}

//==============================================================================

lang_status_t node_to_IR(lang_ctx_t* ctx,
                         node_t*     node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    switch(node->value_type) {
        case OPERATOR: {
            (*_OP(node).to_IR_func)(ctx, node);
            break;
        }
        case IDENTIFIER: {
            var_to_IR(ctx, node);
            break;
        }
        case NUMBER: {
            emit_push(ctx, operand_immersive(node->value.number));
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

lang_status_t var_to_IR(lang_ctx_t* ctx,
                        node_t*     node) // TODO global vars
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    identifier_t var = _ID(node);

    emit_push(ctx, operand_memory(var.addr));

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t passing_func_params_to_IR(lang_ctx_t* ctx,
                                        node_t*     node,
                                        size_t      n_params)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_t* cur_param        = node;
    node_t* params[n_params] = {};

    //--------------------------------------------------------------------------

    for (size_t i = 0; i < n_params; i++) {
        ASSERT(cur_param);
        params[i] = cur_param;
        cur_param = cur_param->right;
    }

    for (size_t i = n_params; i != 0; i--) {
        var_to_IR(ctx, params[i - 1]->left);
    }

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t call_to_IR(lang_ctx_t* ctx,
                         node_t*     node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_t* call         = node;
    node_t* func         = call->left;
    node_t* func_params  = func->left;

    identifier_t func_id = ctx->name_table.ids[func->value.id_index];

    //--------------------------------------------------------------------------

    passing_func_params_to_IR(ctx, func_params, func_id.n_params);

    emit_call(ctx, func_id.name);

    size_t allocated_memory = VAR_SIZE * func_id.n_params;

    emit_add (ctx, operand_register(IR_REG_RSP),
                   operand_immersive(allocated_memory));
    emit_push(ctx, operand_register(IR_REG_RAX));

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t binary_operation_to_IR(lang_ctx_t* ctx,
                                     node_t*     node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_to_IR(ctx, node->left);
    emit_pop(ctx, operand_register(IR_REG_R10));

    node_to_IR(ctx, node->right);
    emit_pop(ctx, operand_register(IR_REG_R11));

    emit_binary_operation(ctx, node->value.operator_code, operand_register(IR_REG_R10),
                                                          operand_register(IR_REG_R11));

    emit_push(ctx, operand_register(IR_REG_R10));

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t assignment_to_IR(lang_ctx_t* ctx,
                               node_t*     node) // TODO global vars
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    identifier_t var = _ID(node->left);

    //--------------------------------------------------------------------------

    node_to_IR(ctx, node->right);

    emit_pop(ctx, operand_memory(var.addr));

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t statement_to_IR(lang_ctx_t* ctx,
                              node_t*     node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    if (node->left) {
        node_to_IR(ctx, node->left);
    }

    if (node->right) {
        node_to_IR(ctx, node->right);
    }

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t new_var_to_IR(lang_ctx_t* ctx,
                            node_t*     node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_t*       assignment = node->left;
    node_t*       var        = assignment->left;
    identifier_t* var_id     = &_ID(var);

    //--------------------------------------------------------------------------

    if (!var_id->addr) {
        var_id->addr = VAR_SIZE * (++ctx->n_locals);
    }

    //--------------------------------------------------------------------------

    assignment_to_IR(ctx, assignment);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t receiving_func_params_to_IR(lang_ctx_t* ctx,
                                          node_t*     node,
                                          size_t      n_params)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_t* cur_param = node;

    for (size_t i = 0; i < n_params; i++) {
        _ID(cur_param->left->left).addr = -VAR_SIZE * ((int) i + 2);
        cur_param = cur_param->right;
    }

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t new_func_to_IR(lang_ctx_t* ctx,
                             node_t*     node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_t*      func_declaration = node;
    node_t*      func             = func_declaration->left;
    node_t*      func_params      = func->left;
    node_t*      func_body        = func->right;
    identifier_t func_id          = _ID(func);

    //--------------------------------------------------------------------------

    emit_global_label(ctx, func_id.name);
    emit_push        (ctx, operand_register(IR_REG_RBP));
    emit_mov         (ctx, operand_register(IR_REG_RBP),
                           operand_register(IR_REG_RSP));

    //--------------------------------------------------------------------------

    size_t sub_rsp_position = ctx->ir_ctx->buffer_size++;

    if (func_params) {
        receiving_func_params_to_IR(ctx, func_params, func_id.n_params);
    }

    node_to_IR(ctx, func_body);

    //--------------------------------------------------------------------------

    size_t cur_position = ctx->ir_ctx->buffer_size;

    ctx->ir_ctx->buffer_size = sub_rsp_position;
    emit_sub(ctx, operand_register(IR_REG_RSP),
                  operand_immersive(VAR_SIZE * ctx->n_locals));

    ctx->ir_ctx->buffer_size = cur_position;

    //--------------------------------------------------------------------------

    ctx->n_locals = 0;
    ctx->level    = 0;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t return_to_IR(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_t* ret       = node;
    node_t* ret_value = ret->left;

    //--------------------------------------------------------------------------

    if (ret_value) {
        node_to_IR(ctx, ret_value);
        emit_pop(ctx, operand_register(IR_REG_RAX));
    }

    size_t allocated_memory = VAR_SIZE * ctx->n_locals;

    emit_add(ctx, operand_register(IR_REG_RSP),
                  operand_immersive(allocated_memory));

    emit_pop(ctx, operand_register(IR_REG_RBP));

    emit_ret(ctx);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t if_to_IR(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_t* statement = node->left;
    node_t* body      = node->right;

    //--------------------------------------------------------------------------

    node_to_IR(ctx, statement);

    emit_pop (ctx, operand_register(IR_REG_RAX));
    emit_test(ctx, operand_register(IR_REG_RAX),
                   operand_register(IR_REG_RAX));

    size_t label_num = ctx->n_labels++;

    emit_je         (ctx, label_num);
    node_to_IR      (ctx, body);
    emit_local_label(ctx, label_num);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t while_to_IR(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_t* statement = node->left;
    node_t* body      = node->right;

    //--------------------------------------------------------------------------

    size_t check_label_num = ctx->n_labels;
    emit_jmp(ctx, check_label_num);

    size_t body_label_num = ctx->n_labels++;
    emit_local_label(ctx, body_label_num);
    node_to_IR      (ctx, body);

    emit_local_label(ctx, check_label_num);
    node_to_IR      (ctx, statement);

    emit_pop (ctx, operand_register(IR_REG_RAX));
    emit_test(ctx, operand_register(IR_REG_RAX),
                   operand_register(IR_REG_RAX));
    emit_jne (ctx, body_label_num);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t in_to_IR(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t exit_to_IR(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    operand_t return_val = operand_register(IR_REG_RAX);
    operand_t exit_code  = operand_immersive(60);

    emit_mov(ctx, operand_register(IR_REG_RAX),
                  operand_immersive(60));
    emit_syscall(ctx);

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

#define _DSL_UNDEF_
#include "dsl.h"

//——————————————————————————————————————————————————————————————————————————————
