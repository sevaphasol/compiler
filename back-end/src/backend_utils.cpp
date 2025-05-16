#include <stdio.h>
// #include <sys/stat.h>
// #include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lang.h"
#include "custom_assert.h"
#include "graph_dump.h"
#include "argv_parser.h"
#include "backend_utils.h"
#include "node_allocator.h"
#include "io_interaction.h"
// #include "ir.h"
// #include "fixup_table.h"
// #include "elf_builder.h"
// #include "buffer.h"
// #include "lib_call_funcs.h"
// #include "backend_utils.h"

#define N_NODES_INIT 1024

//——————————————————————————————————————————————————————————————————————————————

lang_status_t backend_lang_ctx_ctor(lang_ctx_t* ctx, int argc, char* argv[])
{
    ASSERT(ctx);
    ASSERT(argv);

    VERIFY(parse_argv(argc, argv, &ctx->ap_ctx),
           return LANG_OPEN_FILES_ERROR);

    ctx->input_file = fopen(ctx->ap_ctx.input_file, "rb");
    VERIFY(!ctx->input_file, return LANG_FILE_OPEN_ERROR);

    VERIFY(read_in_buf(ctx->input_file,
                       &ctx->input_size,
                       &ctx->code),
           return LANG_READ_CODE_ERROR);

    ctx->nodes = (node_t**) calloc(ctx->input_size + 1, sizeof(node_t*));
    VERIFY(!ctx->nodes,
           return LANG_STD_ALLOCATE_ERROR);
    ctx->n_nodes = 0;
    ctx->pos = 0;

    ctx->name_table.ids = (identifier_t*) calloc(ctx->input_size, sizeof(identifier_t));
    VERIFY(!ctx->name_table.ids, return LANG_STD_ALLOCATE_ERROR);

    ctx->name_table.names = (name_t*) calloc(ctx->input_size,  sizeof(identifier_t));
    VERIFY(!ctx->name_table.names, return LANG_STD_ALLOCATE_ERROR);

    ctx->name_table.n_names = 0;
    ctx->n_globals = 0;
    ctx->n_locals = 0;
    ctx->level = 0;

    VERIFY(node_allocator_ctor(ctx->node_allocator, N_NODES_INIT),
           return LANG_NODE_ALLOCATOR_CTOR_ERROR);

    return LANG_SUCCESS;

}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t backend_lang_ctx_dtor(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    if (ctx->input_file) {
        fclose(ctx->input_file);
        ctx->input_file = nullptr;
    }

    if (ctx->output_file) {
        fclose(ctx->output_file);
        ctx->output_file = nullptr;
    }

    if (ctx->nodes) {
        free(ctx->nodes);
        ctx->nodes = nullptr;
    }

    VERIFY(node_allocator_dtor(ctx->node_allocator),
           return LANG_NODE_ALLOCATOR_DTOR_ERROR);

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t read_input_ctx(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    VERIFY(read_name_table(ctx),
           return LANG_ERROR);

    VERIFY(read_tree(ctx, &ctx->tree),
           return LANG_ERROR);

    VERIFY(graph_dump(ctx, ctx->tree, TREE),
           return LANG_ERROR);

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t read_name_table(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    int n_chars = 0;

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

    for (int i = 0; i < n_ids; i++) {
        sscanf(ctx->code, " { %d %s %d %d %d } %n",
               &len, buf, &type, &is_global, &n_params, &nchars);

        ctx->code += nchars;

        ctx->name_table.ids[i].type      = (identifier_type_t) type;
        ctx->name_table.ids[i].n_params  = n_params;
        ctx->name_table.ids[i].name      = strdup(buf);
        ctx->name_table.ids[i].is_global = is_global;
    }

    sscanf(ctx->code, " %ld%n", &ctx->n_nodes, &nchars);
    ctx->code += nchars;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t read_tree(lang_ctx_t* ctx, node_t** node)
{
    ASSERT(ctx);
    ASSERT(node);

    while (isspace(*ctx->code)) {
        ctx->code++;
    }

    if (*ctx->code == '_') {
        ctx->code++;
        *node = nullptr;

        return LANG_SUCCESS;
    }

    int nchars = 0;
    int type   = 0;
    int val    = 0;

    sscanf(ctx->code, "%*[^0-9] %d %d%n", &type, &val, &nchars);

    value_t node_value = {};

    if(put_node_value(type, val ,&node_value)) {
        return LANG_PUT_NODE_VALUE_ERROR;
    }

    *node = node_ctor(ctx->node_allocator,
                      (value_type_t) type,
                      node_value,
                      0,
                      nullptr,
                      nullptr);

    ctx->code += nchars;

    if (read_tree(ctx, &(*node)->left)) {
        return LANG_READ_LEFT_NODE_ERROR;
    }

    if (read_tree(ctx, &(*node)->right)) {
        return LANG_READ_RIGHT_NODE_ERROR;
    }

    while (isspace(*ctx->code)) {ctx->code++;}

    VERIFY(*(ctx)->code != '}',
           return LANG_INCORRECT_INPUT_SYNTAX_ERROR);

    ctx->code++;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t put_node_value(int type, int val, value_t* node_value)
{
    ASSERT(node_value);

    switch(type) {
        case OPERATOR: {
            node_value->operator_code = (operator_code_t) val;
            break;
        }
        case IDENTIFIER: {
            node_value->id_index = val;
            break;
        }
        case NUMBER: {
            node_value->number = (number_t) val;
            break;
        }
        default: {
            fprintf(stderr, "Unknown type: %d\n", type);

            return LANG_UNKNOWN_TYPE_ERROR;
        }
    }

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————
