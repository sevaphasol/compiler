#include <stack>

#include "lang.h"
#include "custom_assert.h"
#include "ir.h"

#define _DSL_DEFINE_
#include "dsl.h"

//==============================================================================

lang_status_t compile(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    //--------------------------------------------------------------------------

    // VERIFY(asm_globals(ctx),
    //     return LANG_ASM_NODE_ERROR);

    ASM("section .text\n");
    ASM("global _start\n\n");

    ASM("_start:\n");
    ir_emit_label(ctx, "_start");

    ctx->level = 1;

    ASM("call _main\n");
    ir_emit_call(ctx, "_main");

    ASM("mov rax, 60\n");
    ir_emit_mov(ctx, opd_reg(REG_RAX), opd_imm(60));

    ASM("syscall\n");
    ir_emit_syscall(ctx);

    VERIFY(asm_node(ctx, ctx->tree),
           return LANG_ASM_NODE_ERROR);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_node(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    switch(node->value_type) {
        case OPERATOR: {
            (*_OP(node).asm_func)(ctx, node);
            break;
        }
        case IDENTIFIER: {
            asm_var(ctx, node);
            break;
        }
        case NUMBER: {
            ASM("push %ld\n", node->value.number);
            ir_emit_push(ctx, opd_imm(node->value.number));
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

lang_status_t asm_var(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    identifier_t var = _ID(node);

    //--------------------------------------------------------------------------

    if (var.is_global) {
        ASM("push %s\n", var.name)
        return LANG_SUCCESS;
    }

    //--------------------------------------------------------------------------

    if (var.addr < 0) {
        ASM("push qword [rbp + %d] ; func param '%s'\n", -var.addr, var.name);
    } else {
        ASM("push qword [rbp - %d] ; local var '%s'\n",   var.addr, var.name);
    }
    ir_emit_push(ctx, opd_mem(var.addr));

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_passing_func_params(lang_ctx_t* ctx, node_t* node, size_t n_params)
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

    //--------------------------------------------------------------------------

    for (size_t i = n_params; i != 0; i--) {
        asm_var(ctx, params[i - 1]->left);
    }

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_call(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_t* call         = node;
    node_t* func         = call->left;
    node_t* func_params  = func->left;

    identifier_t func_id = ctx->name_table.ids[func->value.id_index];

    //--------------------------------------------------------------------------

    asm_passing_func_params(ctx, func_params, func_id.n_params);

    ASM("call _%s\n", func_id.name);
    ir_emit_call(ctx, func_id.name);

    ASM("add rsp, %ld\n", 8L * func_id.n_params);
    ir_emit_add(ctx, opd_reg(REG_RSP), opd_imm(8 * func_id.n_params));

    ASM("push rax\n");
    ir_emit_push(ctx, opd_reg(REG_RAX));

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_binary_operation(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    asm_node(ctx, node->left);
    ASM("pop r10\n");
    ir_emit_pop(ctx, opd_reg(REG_R10));

    asm_node(ctx, node->right);
    ASM("pop r11\n");
    ir_emit_pop(ctx, opd_reg(REG_R11));

    ASM("%s r10, r11\n", OperatorsTable[node->value.operator_code].asm_name);

    switch (node->value.operator_code) {
        case ADD: {
            ir_emit_add(ctx, opd_reg(REG_R10), opd_reg(REG_R11));
            break;
        }
        case SUB: {
            ir_emit_sub(ctx, opd_reg(REG_R10), opd_reg(REG_R11));
            break;
        }
        case MUL: {
            ir_emit_mul(ctx, opd_reg(REG_R10), opd_reg(REG_R11));
            break;
        }
        case DIV: {
            ir_emit_div(ctx, opd_reg(REG_R10), opd_reg(REG_R11));
            break;
        }
        default: {
            return LANG_ERROR;
        }
    }

    ASM("push r10\n");
    ir_emit_push(ctx, opd_reg(REG_R10));

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_assignment(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    identifier_t var = _ID(node->left);

    //--------------------------------------------------------------------------

    asm_node(ctx, node->right);

    //--------------------------------------------------------------------------

    if (var.is_global) {
        ASM("pop %s\n", var.name);
        return LANG_SUCCESS;
    }

    //--------------------------------------------------------------------------

    if (var.addr < 0) {
        ASM("pop qword [rbp + %d] ; assign func param '%s'\n", -var.addr, var.name);
    } else {
        ASM("pop qword [rbp - %d] ; assign local var '%s'\n",   var.addr, var.name);
    }
    ir_emit_pop(ctx, opd_imm(var.addr));

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_statement(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    if (node->left) {
        asm_node(ctx, node->left);
    }

    if (node->right) {
        asm_node(ctx, node->right);
    }

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_new_var(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_t*       assignment  = node->left;
    node_t*       var         = assignment->left;
    identifier_t* var_id      = &_ID(var);

    //--------------------------------------------------------------------------

    if (!var_id->addr) {
        var_id->addr = VAR_SIZE * (++ctx->n_locals);
    }

    //--------------------------------------------------------------------------

    asm_assignment(ctx, assignment);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_receiving_func_params(lang_ctx_t* ctx, node_t* node, size_t n_params)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_t* cur_param = node;

    for (size_t i = 0; i < n_params; i++) {
        _ID(cur_param->left->left).addr = -VAR_SIZE * (i + 2);
        cur_param = cur_param->right;
    }

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_new_func(lang_ctx_t* ctx, node_t* node)
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

    ASM_NO_TAB("_%s:\n", func_id.name);
    ir_emit_label(ctx, func_id.name);

    ctx->level = 1;

    ASM("push rbp\n");
    ir_emit_push(ctx, opd_reg(REG_RBP));
    ASM("mov rbp, rsp\n");
    ir_emit_mov(ctx, opd_reg(REG_RBP), opd_reg(REG_RSP));

    //--------------------------------------------------------------------------

    ASM("sub rsp,   \n");
    size_t allocated_size_file_pos = ftell(ctx->output_file) - 3;
    size_t allocated_size_pos = ctx->ir_ctx->buffer_size;
    ctx->ir_ctx->buffer_size++;

    //--------------------------------------------------------------------------

    if (func_params) {
        asm_receiving_func_params(ctx, func_params, func_id.n_params);
    }

    asm_node(ctx, func_body);

    //--------------------------------------------------------------------------

    size_t cur_file_pos = ftell(ctx->output_file);
    fseek(ctx->output_file, allocated_size_file_pos, SEEK_SET);

    ASM_NO_TAB("%ld", VAR_SIZE * ctx->n_locals);

    fseek(ctx->output_file, cur_file_pos, SEEK_SET);

    size_t cur_pos = ctx->ir_ctx->buffer_size;
    ctx->ir_ctx->buffer_size = allocated_size_pos;
    ir_emit_sub(ctx, opd_reg(REG_RSP), opd_imm(VAR_SIZE * ctx->n_locals));
    ctx->ir_ctx->buffer_size = cur_pos;

    //--------------------------------------------------------------------------

    ctx->n_locals = 0;
    ctx->level    = 0;

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_return(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_t* ret       = node;
    node_t* ret_value = ret->left;

    //--------------------------------------------------------------------------

    if (ret_value) {
        asm_node(ctx, ret_value);
        ASM("pop rax\n");
        ir_emit_pop(ctx, opd_reg(REG_RAX));
    }

    ASM("add rsp, %ld\n", VAR_SIZE * ctx->n_locals);
    ir_emit_add(ctx, opd_reg(REG_RSP), opd_imm(VAR_SIZE * ctx->n_locals));

    ASM("pop rbp\n");
    ir_emit_pop(ctx, opd_reg(RBP));

    ASM("ret\n");
    ir_emit_ret(ctx);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_unary_operation(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_if(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_t* statement = node->left;
    node_t* body      = node->right;

    //--------------------------------------------------------------------------

    asm_node(ctx, statement);

    ASM("pop rax\n");
    ir_emit_pop(ctx, opd_reg(REG_RAX));
    ASM("test rax, rax\n");
    ir_emit_test(ctx, opd_reg(REG_RAX));

    size_t label_num = ctx->n_labels;

    char label[5] = {};
    snprintf(label, 5, ".L%ld", ctx->n_labels++);

    ASM("je .if_done%ld\n", label_num);
    ir_emit_je(ctx, label);
    asm_node(ctx, body);
    ASM_NO_TAB(".if_done%ld:\n", label_num);
    ir_emit_label(ctx, label);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_while(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    //--------------------------------------------------------------------------

    node_t* statement = node->left;
    node_t* body      = node->right;

    //--------------------------------------------------------------------------

    size_t check_label_num = ctx->n_labels;
    ASM("jmp .check_while_condition%ld\n", check_label_num);

    char label[5] = {};
    snprintf(label, 5, ".L%ld", ctx->n_labels++);

    ir_emit_jmp();

    size_t body_label_num = ctx->n_labels++;
    ASM(".body_while%ld:\n", body_label_num);
    asm_node(ctx, body);

    ASM(".check_while_condition%ld:\n", check_label_num);
    asm_node(ctx, statement);
    ASM("pop rax\n");
    ASM("test rax, rax\n");
    ASM("jne .body_while%ld\n", body_label_num);

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_globals(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_in(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    return LANG_SUCCESS;
}

//==============================================================================

lang_status_t asm_hlt(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    ASM("mov rax, 60\n")
    ASM("syscall\n");

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

#define _DSL_UNDEF_
#include "dsl.h"

//——————————————————————————————————————————————————————————————————————————————
