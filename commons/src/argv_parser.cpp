#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "argv_parser.h"
#include "custom_assert.h"

//——————————————————————————————————————————————————————————————————————————————

inline void print_usage()
{
    fputs("Usage: ./ [options]\n"
          "Options:\n"
          "  --help, -h                 Print this message.\n"
          "  --Source=<filename>, -S    Set assembler dump file name.\n"
          "  --out=<filename>, -o       Set output file name.\n", stderr);
}

//——————————————————————————————————————————————————————————————————————————————

ap_status_t parse_argv(int argc, char *argv[], ap_ctx_t* ctx)
{
//     int opt    = 0;
//     int optind = 0;
//
//     option l_opts[] = {
//         {"help",      no_argument, NULL, 'h'},
//         {"Source",    no_argument, NULL, 'S'},
//         {"input",     no_argument, NULL, 'i'},
//         {"out",       no_argument, NULL, 'o'},
//     };
//
//     const char* s_opts = "hS:i:o:";
//
//     while ((opt = getopt_long(argc, argv, s_opts, l_opts, &optind)) != -1) {
//         switch (opt) {
//             case 'h':
//                 print_usage();
//                 break;
//             case 'i':
//                 ctx->input_file  = optarg;
//                 break;
//             case 'o':
//                 ctx->output_file = optarg;
//                 break;
//             case 'S':
//                 ctx->dump_source = true;
//                 ctx->source_name = optarg;
//                 break;
//             case '?':
//                 print_usage();
//                 return AP_ERROR;
//             default:
//                 print_usage();
//                 return AP_ERROR;
//         }
//     }

    if (!ctx->output_file) {
        ctx->output_file = BackendDefaultOutputName;
    }

    if (!ctx->input_file) {
        ctx->input_file = BackendDefaultInputName;
    }

    ctx->dump_source = true;
    ctx->source_name = "program.s";

    return AP_SUCCESS;
}
