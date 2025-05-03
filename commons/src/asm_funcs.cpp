#include "lang.h"
#include "custom_assert.h"

#define _DSL_DEFINE_
#include "dsl.h"

//===================================================================//

lang_status_t asm_node(lang_ctx_t* ctx, node_t* cur_node)
{
    ASSERT(ctx);
    ASSERT(cur_node);

    //-------------------------------------------------------------------//

    switch(cur_node->value_type)
    {
        case OPERATOR:
        {
            (*OperatorsTable[cur_node->value.operator_code].asm_func)(ctx, cur_node);
            break;
        }
        case IDENTIFIER:
        {
            VERIFY(_ID(cur_node).type != VAR,
                return LANG_ASM_NODE_ERROR);

            asm_var(ctx, cur_node);
            break;
        }
        case NUMBER:
        {
            _PRINT("push %d\n", cur_node->value.number);
            break;
        }
    }

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t asm_var(lang_ctx_t* ctx, node_t* cur_node)
{
    ASSERT(ctx);
    ASSERT(cur_node);

    //-------------------------------------------------------------------//

    identifier_t var = ctx->name_table.ids[cur_node->value.id_index];

    if (var.is_global)
    {
        _PRINT("push [%ld] "
               ";pushing global var %s\n",
                var.addr,
                var.name);
        return LANG_SUCCESS;
    }

    _PRINT("push [BP + %ld] "
           ";pushing local var %s\n",
            var.addr,
            var.name)

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t asm_call(lang_ctx_t* ctx, node_t* cur_node)
{
    ASSERT(ctx);
    ASSERT(cur_node);

    //-------------------------------------------------------------------//

    // _PRINT(";set BP\n");
    _PRINT("push BP\n");

    // _PRINT(";pushing func_params: %d params\n", _ID(cur_node->left).n_params);
    asm_node(ctx, cur_node->left->left);

    // _PRINT(";preparing to call function \"%s\"\n", _ID(cur_node->left).name);
    _PRINT("push BP ;save BP\n");
    _PRINT("push %d\n", _ID(cur_node->left).n_params);
    _PRINT("add\n");
    _PRINT("pop BP\n");

    // _PRINT(";poping func_params to local variables\n");
    for (size_t i = 0; i < _ID(cur_node->left).n_params; i++)
    {
        _PRINT("pop [BP + %ld]\n", _ID(cur_node->left).n_params - i - 1);
    }

    _PRINT("call %s:\n", _ID(cur_node->left).name);
    _PRINT("pop  BP "
           ";new BP\n");
    _PRINT("push AX "
           ";return value (in AX)\n");

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t asm_binary_operation(lang_ctx_t* ctx, node_t* cur_node)
{
    ASSERT(ctx);
    ASSERT(cur_node);
    ASSERT(cur_node->left);
    ASSERT(cur_node->right);

    //-------------------------------------------------------------------//

    // _PRINT(";preparing args for binary operation %s\n",
    //        OperatorsTable[cur_node->value.operator_code].asm_name);
    asm_node(ctx, cur_node->left);
    asm_node(ctx, cur_node->right);
    _PRINT("%s\n", OperatorsTable[cur_node->value.operator_code].asm_name);

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t asm_unary_operation(lang_ctx_t* ctx, node_t* cur_node)
{
    ASSERT(ctx);
    ASSERT(cur_node);

    //-------------------------------------------------------------------//

    // _PRINT(";preparing args for unary operation %s\n",
    //        OperatorsTable[cur_node->value.operator_code].asm_name);
    asm_node(ctx, cur_node->left);
    _PRINT("%s\n", OperatorsTable[cur_node->value.operator_code].asm_name);

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t asm_assignment(lang_ctx_t* ctx, node_t* cur_node)
{
    ASSERT(ctx);
    ASSERT(cur_node);

    //-------------------------------------------------------------------//

    // _PRINT(";assignment\n");
    asm_node(ctx, cur_node->right);

    if (_ID(cur_node->left).is_global)
    {
        _PRINT("pop [%ld]\n", _ID(cur_node->left).addr);
        return LANG_SUCCESS;
    }

    _PRINT("pop [BP + %ld] ;assignment of %s\n",
           _ID(cur_node->left).addr,
           _ID(cur_node->left).name);

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t asm_sequential_action(lang_ctx_t* ctx, node_t* cur_node)
{
    ASSERT(ctx);
    ASSERT(cur_node);

    //-------------------------------------------------------------------//

    // size_t old_n_locals = ctx->n_locals;
    if (cur_node->left)  asm_node(ctx, cur_node->left);

    // if (OperatorsTable[cur_node->value.operator_code].code == STATEMENT)
    // {
    //     ctx->n_locals = old_n_locals;
    // }

    if (cur_node->right) asm_node(ctx, cur_node->right);

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t asm_if(lang_ctx_t* ctx, node_t* cur_node)
{
    ASSERT(ctx);
    ASSERT(cur_node);

    //-------------------------------------------------------------------//

    // _PRINT(";pushing if params\n");
    asm_node(ctx, cur_node->left);

    _PRINT("push 0\n");
    _PRINT("je else_body_%ld:\n", ctx->n_labels);

    ctx->level++;
    // _PRINT(";if body\n");
    asm_node(ctx, cur_node->right);
    ctx->level--;

    _PRINT("else_body_%ld:\n", ctx->n_labels++);
    ctx->level++;

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t asm_while(lang_ctx_t* ctx, node_t* cur_node)
{
    ASSERT(ctx);
    ASSERT(cur_node);

    //-------------------------------------------------------------------//

    // _PRINT("pushing while params\n");

    _PRINT("start_of_while_%ld:\n", ctx->n_labels);

    asm_node(ctx, cur_node->left);
    _PRINT("push 0\n");
    _PRINT("je end_of_while_%ld:\n", ctx->n_labels);

    ctx->level++;
    asm_node(ctx, cur_node->right);
    ctx->level--;

    _PRINT("jmp start_of_while_%ld:\n", ctx->n_labels);
    _PRINT("end_of_while_%ld:\n", ctx->n_labels);

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t asm_new_var(lang_ctx_t* ctx, node_t* cur_node)
{
    ASSERT(ctx);
    ASSERT(cur_node);

    static int n_calls = 0;
    n_calls++;

    //-------------------------------------------------------------------//

    node_t* var = nullptr;

    if (_NODE_IS_OPERATOR(cur_node->left, ASSIGNMENT))
    {
        var = cur_node->left->left;
    }
    else if (_NODE_IS_ID_TYPE(cur_node->left, VAR))
    {
        var = cur_node->left;
    }
    else
    {
        fprintf(stderr, "DEBUG in %s:%d:%s\n",
                        __FILE__, __LINE__, __PRETTY_FUNCTION__);

        return LANG_ASM_NEW_VAR_ERROR;
    }

    // _PRINT(";allocating static memory for variable \"%s\"\n",
    //         _ID(var).name);

    //-------------------------------------------------------------------//

    if (_ID(var).is_global) { _ID(var).addr = ctx->n_globals++; }
    else                    { _ID(var).addr = ctx->n_locals++;  }

    //-------------------------------------------------------------------//

    if (_NODE_IS_OPERATOR(cur_node->left, ASSIGNMENT))
    {
        asm_assignment(ctx, cur_node->left);
    }

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t asm_new_func(lang_ctx_t* ctx, node_t* cur_node)
{
    ASSERT(ctx);
    ASSERT(cur_node);

    //-------------------------------------------------------------------//

    ctx->level = 0;

    size_t old_n_locals = ctx->n_locals;
    ctx->n_locals = 0;

    // _PRINT("\n;//=========== declaration of function %s ===========//\n\n",
    //        _ID(cur_node->left).name);
    _PRINT("%s:\n", _ID(cur_node->left).name);

    if (cur_node->left->left) asm_node(ctx, cur_node->left->left);
    ctx->level++;
    asm_node(ctx, cur_node->left->right);
    ctx->level--;

    ctx->n_locals = old_n_locals;

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t asm_return(lang_ctx_t* ctx, node_t* cur_node)
{
    ASSERT(ctx);
    ASSERT(cur_node);

    //-------------------------------------------------------------------//

    if (cur_node->left) asm_node(ctx, cur_node->left);
    _PRINT("pop AX ;pop in AX return value\n");
    _PRINT("ret\n");

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t asm_in(lang_ctx_t* ctx, node_t* cur_node)
{
    ASSERT(ctx);
    ASSERT(cur_node);

    //-------------------------------------------------------------------//

    _PRINT("in\n");

    if (_ID(cur_node->left->left).is_global)
    {
        _PRINT("pop [%ld]\n", _ID(cur_node->left->left).addr);
        return LANG_SUCCESS;
    }

    _PRINT("pop [BP + %ld]\n", _ID(cur_node->left->left).addr);

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t asm_hlt(lang_ctx_t* ctx, node_t* cur_node)
{
    ASSERT(ctx);
    ASSERT(cur_node);

    //-------------------------------------------------------------------//

    _PRINT("hlt\n\n");

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//———————————————————————————————————————————————————————————————————//

#define _DSL_UNDEF_
#include "dsl.h"

//———————————————————————————————————————————————————————————————————//
