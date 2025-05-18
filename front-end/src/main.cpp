#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include "custom_assert.h"
#include "graph_dump.h"
#include "node_allocator.h"
#include "io_interaction.h"
#include "color_print.h"
#include "read_name_table_utils.h"

//———————————————————————————————————————————————————————————————————//

#define _DSL_DEFINE_
#include "dsl.h"

//———————————————————————————————————————————————————————————————————//

extern lang_status_t tokenize        (lang_ctx_t* ctx);
extern lang_status_t syntax_analysis (lang_ctx_t* ctx);

//———————————————————————————————————————————————————————————————————//

int main(int argc, const char* argv[])
{
    lang_ctx_t ctx = {};

    node_allocator_t node_allocator = {};
    ctx.node_allocator = &node_allocator;

    //---------------------------------------------------------------//

    VERIFY(lang_ctx_ctor(&ctx,
                         argc,
                         argv,
                         FrontendDefaultInput,
                         FrontendDefaultOutput),
           lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    //---------------------------------------------------------------//

    VERIFY(tokenize(&ctx),
           lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    //-------------------------------------------------------------------//

    VERIFY(syntax_analysis(&ctx),
           lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    //---------------------------------------------------------------//

    VERIFY(name_table_output(&ctx),
           lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    //---------------------------------------------------------------//

    VERIFY(tree_output(&ctx, ctx.nodes[0]),
           lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    //---------------------------------------------------------------//

    VERIFY(graph_dump(&ctx, ctx.nodes[0], TREE),
           lang_ctx_dtor(&ctx);
           return EXIT_FAILURE);

    //---------------------------------------------------------------//

    VERIFY(lang_ctx_dtor(&ctx),
           return EXIT_FAILURE);

    //---------------------------------------------------------------//

    fprintf(stderr, _PURPLE("front-end:  ") _GREEN("success\n"));

    //-------------------------------------------------------------------//

    return EXIT_SUCCESS;
}

//———————————————————————————————————————————————————————————————————//

#define _DSL_UNDEF_
#include "dsl.h"

//———————————————————————————————————————————————————————————————————//
