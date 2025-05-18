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
#include "ir.h"
#include "fixup_table.h"
#include "elf_builder.h"
#include "buffer.h"
#include "lib_call_funcs.h"
#include "read_name_table_utils.h"
#include "encode_utils.h"
#include "color_print.h"

//——————————————————————————————————————————————————————————————————————————————

static lang_status_t backend_lang_ctx_ctor (lang_ctx_t* ctx,
                                            int         argc,
                                            char*       argv[]);

static lang_status_t backend_lang_ctx_dtor (lang_ctx_t* ctx);

static lang_status_t compile    (lang_ctx_t* ctx);
static lang_status_t make_ir    (lang_ctx_t* ctx);
static lang_status_t make_asm   (lang_ctx_t* ctx);
static lang_status_t make_binary(lang_ctx_t* ctx);

extern lang_status_t optimize_ir(lang_ctx_t* ctx);

//——————————————————————————————————————————————————————————————————————————————

int main(int argc, char* argv[])
{
    lang_ctx_t ctx = {};

    node_allocator_t node_allocator;
    ctx.node_allocator = &node_allocator;

    VERIFY(backend_lang_ctx_ctor(&ctx, argc, argv),
           return EXIT_FAILURE);

    VERIFY(read_input_ctx(&ctx),
           backend_lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    VERIFY(compile(&ctx),
           backend_lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    VERIFY(backend_lang_ctx_dtor(&ctx),
           return EXIT_FAILURE);

    fprintf(stderr, _PURPLE("back-end:   ") _GREEN("success\n"));

    return EXIT_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t compile(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    VERIFY(make_ir(ctx),     return LANG_ERROR);
    VERIFY(optimize_ir(ctx), return LANG_ERROR);

    if (ctx->ap_ctx.dump_source) {
        VERIFY(make_asm(ctx), return LANG_ERROR);
    }

    VERIFY(make_binary(ctx), return LANG_ERROR);

    VERIFY(create_elf_file(ctx->ap_ctx.output_file,
                           ctx->bin_buf.data,
                           ctx->bin_buf.size),
           return LANG_ERROR);

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t make_ir(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    buf_ctor(&ctx->ir_buf, IR_BUFFER_INIT_CAPACITY);
    build_ir(ctx);
    ir_buffer_graph_dump(ctx, (ir_instr_t*) &ctx->ir_buf.data);

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t make_asm(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    ctx->output_file = fopen(ctx->ap_ctx.source_name, "wb");
    VERIFY(!ctx->output_file, return LANG_FILE_OPEN_ERROR);

    ir_to_asm(ctx);

    fclose(ctx->output_file);

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t make_binary(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    buf_ctor(&ctx->bin_buf, BIN_BUF_INIT_CAPACITY);

    label_table_ctor    (&ctx->label_table,     TABLE_INIT_CAPACITY);
    fixup_table_ctor    (&ctx->fixups,          TABLE_INIT_CAPACITY);
    lib_calls_table_ctor(&ctx->lib_calls_table, TABLE_INIT_CAPACITY);

    ir_to_binary(ctx);

    buf_dtor(&ctx->ir_buf);

    label_table_dtor(&ctx->label_table);
    fixup_table_dtor(&ctx->fixups);

    if (ctx->lib_calls_table.load_in ||
        ctx->lib_calls_table.load_out) {
        solve_lib_call_requests(ctx);
    }

    lib_calls_table_dtor(&ctx->lib_calls_table);

    return LANG_SUCCESS;
}

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
