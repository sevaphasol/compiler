#include <math.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lang.h"
#include "custom_assert.h"
#include "graph_dump.h"
#include "node_allocator.h"
#include "io_interaction.h"

//———————————————————————————————————————————————————————————————————//

#define _DSL_DEFINE_
#include "dsl.h"

//———————————————————————————————————————————————————————————————————//

static lang_status_t read_tree(lang_ctx_t* ctx, node_t** node);
static lang_status_t put_node_value(int type, int val, value_t* node_value);
static lang_status_t read_name_table(lang_ctx_t* ctx);
static lang_status_t simplify_node(lang_ctx_t* ctx, node_t* node);
static lang_status_t try_calc(node_t* tree);
static lang_status_t simplify_opr(node_t* tree);
static lang_status_t calc_node(node_t* node, number_t l, number_t r);
static bool          is_math_operation(operator_code_t code);
static lang_status_t tree_output       (lang_ctx_t* ctx,
                                        node_t*     node);

static lang_status_t name_table_output (lang_ctx_t* ctx);

static lang_status_t print_node_value  (FILE*       fp,
                                        node_t*     node);

//———————————————————————————————————————————————————————————————————//

int main(int argc, const char* argv[])
{
    lang_ctx_t ctx = {};

    node_allocator_t node_allocator;
    ctx.node_allocator = &node_allocator;

    //-------------------------------------------------------------------//

    VERIFY(lang_ctx_ctor(&ctx,
                         argc,
                         argv,
                         ReverseFrontendDefaultInput,
                         ReverseFrontendDefaultOutput),
           return EXIT_FAILURE);

    //-------------------------------------------------------------------//

    VERIFY(read_name_table(&ctx),
           lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    //-------------------------------------------------------------------//

    VERIFY(read_tree(&ctx, &ctx.tree),
           lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    //-------------------------------------------------------------------//

    VERIFY(graph_dump(&ctx, ctx.tree, TREE),
           lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    //-------------------------------------------------------------------//

    VERIFY(simplify_node(&ctx, ctx.tree),
           lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    //-------------------------------------------------------------------//

    VERIFY(graph_dump(&ctx, ctx.tree, TREE),
           lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    //-------------------------------------------------------------------//

    VERIFY(name_table_output(&ctx),
           lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    //---------------------------------------------------------------//

    VERIFY(tree_output(&ctx, ctx.tree),
           lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    //-------------------------------------------------------------------//

    printf(_GREEN("Success\n"));

    //-------------------------------------------------------------------//

    return EXIT_SUCCESS;
}

//===================================================================//

lang_status_t simplify_node(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    VERIFY(!node,
           return LANG_SRC_NODE_ERROR);

    //-------------------------------------------------------------------//

    switch (node->value_type)
    {
        case NUMBER: break;
        case IDENTIFIER: break;
        case OPERATOR:
        {
            VERIFY(simplify_opr(node) == LANG_TRY_CALC_ERROR,
                   return LANG_SIMPLIFY_OPR_CODE_ERROR);
            break;
        }

        default:
        {
            fprintf(stderr, "Invalid type\n");
            return LANG_SOURCE_CODE_ERROR;
        }
    }

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t name_table_output(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    //---------------------------------------------------------------//

    fprintf(ctx->output_file, "%ld\n", ctx->name_table.n_ids);

    for (int i = 0; i < ctx->name_table.n_ids; i++)
    {
        fprintf(ctx->output_file,
               "{ %-3ld %-10s %-1d %-1d %-2d }\n",
               ctx->name_table.ids[i].len,
               ctx->name_table.ids[i].name,
               ctx->name_table.ids[i].type,
               ctx->name_table.ids[i].is_global ? 1 : 0,
               ctx->name_table.ids[i].n_params);
    }

    fputc('\n', ctx->output_file);
    fprintf(ctx->output_file, "%ld\n", ctx->n_nodes);

    //---------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t tree_output(lang_ctx_t* ctx, node_t* node)
{
    ASSERT(ctx);
    ASSERT(node);

    //---------------------------------------------------------------//

    fprintf(ctx->output_file, "{%d ", node->value_type);

    VERIFY(print_node_value(ctx->output_file, node),
           return LANG_TREE_OUTPUT_ERROR);

    node->left  ? tree_output(ctx, node->left)  : fputs(" _ ", ctx->output_file);
    node->right ? tree_output(ctx, node->right) : fputs(" _ ", ctx->output_file);

    fputs("} ",  ctx->output_file);

    //---------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t print_node_value(FILE* fp, node_t* node)
{
    ASSERT(fp);
    ASSERT(node);

    //---------------------------------------------------------------//

    switch (node->value_type)
    {
        case OPERATOR:
        {
            fprintf(fp, "%d ", node->value.operator_code);

            break;
        }
        case IDENTIFIER:
        {
            fprintf(fp, "%ld ", node->value.id_index);

            break;
        }
        case NUMBER:
        {
            fprintf(fp, "%d ", node->value.number);

            break;
        }
        default:
        {
            return LANG_PRINT_NODE_VALUE_ERROR;

            break;
        }
    }

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t try_calc(node_t* tree)
{
    ASSERT(tree);

    //-------------------------------------------------------------------//

    switch (tree->value_type)
    {
        case NUMBER:     return LANG_SUCCESS;
        case IDENTIFIER:
        {
            if (tree->left)  simplify_opr(tree->left);
            if (tree->right) simplify_opr(tree->right);

            return LANG_NOT_CONSTANT_STATUS;
        }
        case OPERATOR:   return simplify_opr(tree);

        default:
        {
            fprintf(stderr, "Undefined type\n");
            break;
        }
    }

    //-------------------------------------------------------------------//

    return LANG_TRY_CALC_ERROR;
}

//===================================================================//

lang_status_t simplify_opr(node_t* tree)
{
    ASSERT(tree);

    //-------------------------------------------------------------------//

    if (tree->left && tree->right)
    {
        try_calc(tree->left);
        try_calc(tree->right);

        //-------------------------------------------------------------------//

        if (tree->left->value_type  == NUMBER &&
            tree->right->value_type == NUMBER &&
            is_math_operation(tree->value.operator_code))
        {
            calc_node(tree, tree->left->value.number,
                            tree->right->value.number);

            return LANG_SUCCESS;
        }

        return LANG_NOT_CONSTANT_STATUS;
    }

    if (tree->left)
    {
        try_calc(tree->left);

        //-------------------------------------------------------------------//

        if (tree->left->value_type == NUMBER &&
            is_math_operation(tree->value.operator_code))
        {
            calc_node(tree, tree->left->value.number, 0);

            return LANG_SUCCESS;
        }

        //-------------------------------------------------------------------//

        return LANG_NOT_CONSTANT_STATUS;
    }

    //-------------------------------------------------------------------//

    return LANG_TRY_CALC_ERROR;
}

//===================================================================//

lang_status_t calc_node(node_t* node, number_t l, number_t r)
{
    ASSERT(node);
    ASSERT(node->value_type == OPERATOR);

    //-------------------------------------------------------------------//

    operator_code_t opr_code = node->value.operator_code;

    switch (node->value.operator_code)
    {
        case ADD:  node->value.number = l + r;                break;
        case SUB:  node->value.number = l - r;                break;
        case MUL:  node->value.number = l * r;                break;
        case DIV:  node->value.number = l / r;                break;
        case SQRT: node->value.number = (number_t) sqrt(l);   break;
        case POW:  node->value.number = (number_t) pow(l, r); break;
        case SIN:  node->value.number = (number_t) sin(l);    break;
        case COS:  node->value.number = (number_t) cos(l);    break;
        default:   return LANG_SUCCESS;
    }

    //-------------------------------------------------------------------//

    printf(_GREEN("Simplifying: ") " operation %s with params %2d, %2d ———> %d\n",
           OperatorsTable[opr_code].asm_name,
           l, r,
           node->value.number
           );

    //-------------------------------------------------------------------//

    node->value_type = NUMBER;
    node->left       = nullptr;
    node->right      = nullptr;

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

bool is_math_operation(operator_code_t code)
{
    return (code == ADD  ||
            code == SUB  ||
            code == MUL  ||
            code == DIV  ||
            code == SQRT ||
            code == POW  ||
            code == SIN  ||
            code == COS);
}

//===================================================================//

lang_status_t read_name_table(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    //-------------------------------------------------------------------//

    int n_chars = 0;

    //-------------------------------------------------------------------//

    size_t n_ids = 0;
    sscanf(ctx->code, "%ld%n", &n_ids, &n_chars);
    ctx->code += n_chars;
    ctx->name_table.n_ids = n_ids;

    ctx->name_table.ids = (identifier_t*) calloc(n_ids, sizeof(identifier_t));
    VERIFY(!ctx->name_table.ids,
           return LANG_STD_ALLOCATE_ERROR);

    int  len = 0;
    char buf[MaxStrLength] = {};
    int type = 0;
    int n_params = 0;
    int is_global;
    int nchars = 0;

    for (int i = 0; i < n_ids; i++)
    {
        sscanf(ctx->code, " { %d %s %d %d %d } %n",
                          &len,
                          buf,
                          &type,
                          &is_global,
                          &n_params,
                          &nchars);

        ctx->code += nchars;

        ctx->name_table.ids[i].len       = len;
        ctx->name_table.ids[i].type      = (identifier_type_t) type;
        ctx->name_table.ids[i].n_params  = n_params;
        ctx->name_table.ids[i].name      = strdup(buf);
        ctx->name_table.ids[i].is_global = is_global;
    }

    sscanf(ctx->code, " %ld%n", &ctx->n_nodes, &nchars);
    ctx->code += nchars;

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t read_tree(lang_ctx_t* ctx, node_t** node)
{
    ASSERT(ctx);
    ASSERT(node);

    //-------------------------------------------------------------------//

    while (isspace(*ctx->code)) {ctx->code++;}

    //-------------------------------------------------------------------//

    if (*ctx->code == '_')
    {
        ctx->code++;

        *node = nullptr;

        return LANG_SUCCESS;
    }

    //-------------------------------------------------------------------//

    int nchars = 0;
    int type   = 0;
    int val    = 0;

    sscanf(ctx->code, "%*[^0-9] %d %d%n", &type, &val, &nchars);

    //-------------------------------------------------------------------//

    value_t node_value = {};

    VERIFY(put_node_value(type, val ,&node_value),
           return LANG_PUT_NODE_VALUE_ERROR);

    //-------------------------------------------------------------------//

    *node = node_ctor(ctx->node_allocator,
                      (value_type_t) type,
                      node_value,
                      0,
                      nullptr,
                      nullptr);

    //-------------------------------------------------------------------//

    ctx->code += nchars;

    VERIFY(read_tree(ctx, &(*node)->left),
           return LANG_READ_LEFT_NODE_ERROR);

    VERIFY(read_tree(ctx, &(*node)->right),
           return LANG_READ_RIGHT_NODE_ERROR);

    //-------------------------------------------------------------------//

    while (isspace(*ctx->code)) {ctx->code++;}

    VERIFY(*(ctx)->code != '}',
           return LANG_INCORRECT_INPUT_SYNTAX_ERROR);

    ctx->code++;

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

lang_status_t put_node_value(int type, int val, value_t* node_value)
{
    ASSERT(node_value);

    //-------------------------------------------------------------------//

    switch(type)
    {
        case OPERATOR:
        {
            node_value->operator_code = (operator_code_t) val;
            break;
        }
        case IDENTIFIER:
        {
            node_value->id_index = val;
            break;
        }
        case NUMBER:
        {
            node_value->number = (number_t) val;
            break;
        }
        default:
        {
            fprintf(stderr, "Unknown type: %d\n", type);

            return LANG_UNKNOWN_TYPE_ERROR;
        }
    }

    //-------------------------------------------------------------------//

    return LANG_SUCCESS;
}

//===================================================================//

#define _DSL_UNDEF_
#include "dsl.h"

//———————————————————————————————————————————————————————————————————//
