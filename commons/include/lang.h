#ifndef LANG_H__
#define LANG_H__

//———————————————————————————————————————————————————————————————————//

#include <stdio.h>

//———————————————————————————————————————————————————————————————————//

const int MaxStrLength = 100;

const char* const FrontendDefaultInput  = "../examples/factorial_source.txt";
const char* const FrontendDefaultOutput = "../examples/factorial_frontend.txt";

const char* const BackendDefaultInput   = "../examples/factorial_frontend.txt";
const char* const BackendDefaultOutput  = "../examples/factorial_backend.asm";

const char* const ReverseFrontendDefaultInput  = "../examples/factorial_frontend.txt";
const char* const ReverseFrontendDefaultOutput = "factorial_source.txt";

//———————————————————————————————————————————————————————————————————//

enum lang_status_t
{
    LANG_SUCCESS                         = 0,
    LANG_ERROR                           = 1,
    LANG_READ_CODE_ERROR                 = 2,
    LANG_PARSE_ARGV_ERROR                = 3,
    LANG_FILE_OPEN_ERROR                 = 4,
    LANG_GET_FILE_SIZE_ERROR             = 5,
    LANG_STD_ALLOCATE_ERROR              = 6,
    LANG_FREAD_ERROR                     = 7,
    LANG_FCLOSE_ERROR                    = 8,
    LANG_NODE_ALLOCATOR_CTOR_ERROR       = 9,
    LANG_TOKENIZER_CTX_CTOR_ERROR        = 10,
    LANG_SYNTAX_ERROR                    = 11,
    LANG_OPEN_FILES_ERROR                = 12,
    LANG_GET_TOKEN_ERROR                 = 13,
    LANG_NODE_ALLOCATOR_DTOR_ERROR       = 14,
    LANG_PRINT_NODE_VALUE_ERROR          = 15,
    LANG_TREE_OUTPUT_ERROR               = 16,
    LANG_PUT_NODE_VALUE_ERROR            = 17,
    LANG_UNKNOWN_TYPE_ERROR              = 18,
    LANG_READ_LEFT_NODE_ERROR            = 19,
    LANG_READ_RIGHT_NODE_ERROR           = 20,
    LANG_INCORRECT_INPUT_SYNTAX_ERROR    = 21,
    LANG_ASM_NODE_ERROR                  = 22,
    LANG_UNKNOWN_OPERATOR_ERROR          = 23,
    LANG_GET_STR_NODE_ERROR              = 24,
    LANG_GET_DECLARATION_ERROR           = 25,
    LANG_GET_GLOBAL_STATEMENT_ERROR      = 26,
    LANG_GET_FUNC_PARAMS_ERROR           = 27,
    LANG_GET_BODY_ERROR                  = 28,
    LANG_GET_EXPRESSION_ERROR            = 29,
    LANG_GET_STATEMENT_ERROR             = 30,
    LANG_GET_STANDART_FUNC_ERROR         = 31,
    LANG_GET_ASSIGNMENT_ERROR            = 32,
    LANG_GET_IF_ERROR                    = 33,
    LANG_GET_RETURN_ERROR                = 34,
    LANG_GET_FUNC_ERROR                  = 35,
    LANG_GET_SCAN_ERROR                  = 36,
    LANG_GET_VAR_DECLARATION_ERROR       = 37,
    LANG_GET_CALL_ERROR                  = 38,
    LANG_GET_MUL_DIV_EXPRESSION_ERROR    = 39,
    LANG_GET_MUL_DIV_EXPRESSION          = 40,
    LANG_GET_SINGLE_EXPRESSION_ERROR     = 41,
    LANG_NODES_OVERFLOW_ERROR            = 42,
    LANG_GET_IN_PARENT_EXPRESSION_ERROR  = 43,
    LANG_GET_FUNC_USE_PARAMS_ERROR       = 44,
    LANG_GET_FUNC_DECLARATION_ERROR      = 45,
    LANG_GET_VAR_DECLARATION             = 46,
    LANG_ID_STACK_OVERFLOW_ERROR         = 47,
    LANG_ID_STACK_UNDERFLOW_ERROR        = 48,
    LANG_POP_LOCALES_ERROR               = 49,
    LANG_REDECLARATION_ERROR             = 50,
    LANG_NOT_INIT_ERROR                  = 51,
    LANG_PUSH_NEW_ID_COUNTER_ERROR       = 52,
    LANG_ID_COUNTER_STACK_OVERFLOW_ERROR = 53,
    LANG_ADD_NEW_ID_ERROR                = 54,
    LANG_ASM_NEW_VAR_ERROR               = 55,
    LANG_GET_HLT_ERROR                   = 56,
    LANG_OP_SOURCE_CODE_ERROR            = 57,
    LANG_SOURCE_CODE_ERROR               = 58,
    LANG_BINARY_OP_SOURCE_ERROR          = 59,
    LANG_UNARY_OP_SOURCE_ERROR           = 60,
    LANG_SEQ_OP_SOURCE_ERROR             = 61,
    LANG_OP_WITH_BODY_SOURCE_ERROR       = 62,
    LANG_ZEROARY_OP_SOURCE_ERROR         = 63,
    LANG_CALL_OP_SOURCE_ERROR            = 64,
    LANG_NEW_FUNC_SOURCE_ERROR           = 65,
    LANG_SRC_NODE_ERROR                  = 66,
    LANG_SRC_BINARY_OP_ERROR             = 67,
    LANG_SRC_UNARY_OP_ERROR              = 68,
    LANG_SRC_NEW_FUNC_ERROR              = 69,
    LANG_SRC_STATEMENT_ERROR             = 70,
    LANG_SRC_COND_ERROR                  = 71,
    LANG_SRC_RET_ERROR                   = 72,
    LANG_SRC_PARAM_LINKER_ERROR          = 73,
    LANG_SRC_NEW_VAR_ERROR               = 74,
    LANG_SRC_IN_ERROR                    = 75,
    LANG_SRC_OUT_ERROR                   = 76,
    LANG_SRC_CALL_ERROR                  = 77,
    LANG_SIMPLIFY_OPR_CODE_ERROR         = 78,
    LANG_NOT_CONSTANT_STATUS             = 79,
    LANG_TRY_CALC_ERROR                  = 80,
};

//———————————————————————————————————————————————————————————————————//

enum value_type_t
{
    POISON     = 0,
    OPERATOR   = 1,
    NUMBER     = 2,
    IDENTIFIER = 3,
};

//———————————————————————————————————————————————————————————————————//

enum operator_code_t
{
    UNDEFINED     = 0,
    ADD           = 1,
    SUB           = 2,
    MUL           = 3,
    DIV           = 4,
    COS           = 5,
    SIN           = 6,
    SQRT          = 7,
    POW           = 8,
    BIGGER        = 9,
    SMALLER       = 10,
    ASSIGNMENT    = 11,
    OPEN_BRACKET  = 12,
    CLOSE_BRACKET = 13,
    BODY_START    = 14,
    BODY_END      = 15,
    STATEMENT     = 16,
    IF            = 17,
    WHILE         = 18,
    RET           = 19,
    PARAM_LINKER  = 20,
    NEW_VAR       = 21,
    NEW_FUNC      = 22,
    IN            = 23,
    OUT           = 24,
    CALL          = 25,
    HLT           = 26,
};

//———————————————————————————————————————————————————————————————————//

struct lang_ctx_t;
struct node_t;

struct operator_t
{
    operator_code_t code;
    const char*     name;
    size_t          len;
    const char*     asm_name;
    int             n_children;
    lang_status_t   (*asm_func) (lang_ctx_t* ctx, node_t* cur_node);
    lang_status_t   (*src_func) (lang_ctx_t* ctx, node_t* cur_node);
};

//———————————————————————————————————————————————————————————————————//

lang_status_t asm_binary_operation  (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t asm_assignment        (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t asm_sequential_action (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t asm_if                (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t asm_while             (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t asm_new_var           (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t asm_new_func          (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t asm_return            (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t asm_unary_operation   (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t asm_in                (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t asm_call              (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t asm_hlt               (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t asm_var               (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t asm_node              (lang_ctx_t* ctx, node_t* cur_node);

//———————————————————————————————————————————————————————————————————//

lang_status_t src_node              (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t src_binary_op         (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t src_unary_op          (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t src_new_func          (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t src_statement         (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t src_cond              (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t src_ret               (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t src_param_linker      (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t src_new_var           (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t src_in                (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t src_out               (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t src_call              (lang_ctx_t* ctx, node_t* cur_node);
lang_status_t src_hlt               (lang_ctx_t* ctx, node_t* cur_node);

//———————————————————————————————————————————————————————————————————//

#define STR_AND_LEN(str) str, sizeof(str) / sizeof(char)

const operator_t OperatorsTable[] =
{ // code           name and len              asm_name    n_childs       asm_func                src_func
    {UNDEFINED,     nullptr, 0,               nullptr,       0,          nullptr               , nullptr,           },
    {ADD,           STR_AND_LEN("+"),         "add",         2,          &asm_binary_operation , &src_binary_op,    },
    {SUB,           STR_AND_LEN("-"),         "sub",         2,          &asm_binary_operation , &src_binary_op,    },
    {MUL,           STR_AND_LEN("*"),         "mul",         2,          &asm_binary_operation , &src_binary_op,    },
    {DIV,           STR_AND_LEN("/"),         "div",         2,          &asm_binary_operation , &src_binary_op,    },
    {COS,           STR_AND_LEN("cosipinus"), "cos",         1,          &asm_unary_operation  , &src_unary_op,     },
    {SIN,           STR_AND_LEN("sipinus"),   "sin",         1,          &asm_unary_operation  , &src_unary_op,     },
    {SQRT,          STR_AND_LEN("sqrt"),      "sqrt",        1,          &asm_unary_operation  , &src_unary_op,     },
    {POW,           STR_AND_LEN("^"),         "pow",         1,          &asm_unary_operation  , &src_binary_op,    },
    {BIGGER,        STR_AND_LEN(">"),         nullptr,       2,          &asm_assignment       , &src_binary_op,    },
    {SMALLER,       STR_AND_LEN("<"),         nullptr,       2,          &asm_assignment       , &src_binary_op,    },
    {ASSIGNMENT,    STR_AND_LEN("="),         nullptr,       2,          &asm_assignment       , &src_binary_op,    },
    {OPEN_BRACKET,  STR_AND_LEN("("),         nullptr,       1,          nullptr               , nullptr,           },
    {CLOSE_BRACKET, STR_AND_LEN(")"),         nullptr,       1,          nullptr               , nullptr,           },
    {BODY_START,    STR_AND_LEN("{"),         nullptr,       1,          nullptr               , nullptr,           },
    {BODY_END,      STR_AND_LEN("}"),         nullptr,       1,          nullptr               , nullptr,           },
    {STATEMENT,     STR_AND_LEN("sosal?"),    nullptr,       2,          &asm_sequential_action, &src_statement,    },
    {IF,            STR_AND_LEN("if"),        nullptr,       2,          &asm_if               , &src_cond,         },
    {WHILE,         STR_AND_LEN("while"),     nullptr,       2,          &asm_while            , &src_cond,         },
    {RET,           STR_AND_LEN("buyTNF"),    nullptr,       0,          &asm_return           , &src_ret,          },
    {PARAM_LINKER,  STR_AND_LEN("krasivaya"), nullptr,       2,          &asm_sequential_action, &src_param_linker, },
    {NEW_VAR,       STR_AND_LEN("krosovka"),  nullptr,       2,          &asm_new_var          , &src_new_var,      },
    {NEW_FUNC,      STR_AND_LEN("korobka"),   nullptr,       2,          &asm_new_func         , &src_new_func,     },
    {IN,            STR_AND_LEN("scan"),      "in",          1,          &asm_in               , &src_in,           },
    {OUT,           STR_AND_LEN("print"),     "out",         1,          &asm_unary_operation  , &src_out,          },
    {CALL,          STR_AND_LEN("please"),    "call",        0,          &asm_call             , &src_call,         },
    {HLT,           STR_AND_LEN("sosal!"),    nullptr,       0,          &asm_hlt              , &src_hlt,          }
};

#undef STR_AND_LEN

const int nOperators = sizeof(OperatorsTable) / sizeof(operator_t);

//———————————————————————————————————————————————————————————————————//

enum identifier_type_t
{
    UNKNOWN = 0,
    VAR     = 1,
    FUNC    = 2,
};

//———————————————————————————————————————————————————————————————————//

struct identifier_t
{
    identifier_type_t type;
    char*             name;
    size_t            len;
    int               n_params;
    bool              is_inited;
    size_t            addr;
    bool              is_global;
};

//———————————————————————————————————————————————————————————————————//

struct name_t
{
    char* name;
    size_t len;
};

//———————————————————————————————————————————————————————————————————//

typedef int number_t;

//———————————————————————————————————————————————————————————————————//

union value_t
{
    operator_code_t   operator_code;
    size_t            id_index;
    number_t          number;
};

//———————————————————————————————————————————————————————————————————//

struct node_t
{
    value_type_t value_type;
    value_t      value;
    size_t       line_number;
    node_t*      left;
    node_t*      right;
};

//———————————————————————————————————————————————————————————————————//

struct stack_t
{
    size_t  size;
    size_t  top;
    size_t* data;
};

//———————————————————————————————————————————————————————————————————//

struct name_table_t
{
    size_t        n_names;
    name_t*       names;
    size_t        n_ids;
    identifier_t* ids;
};

//———————————————————————————————————————————————————————————————————//

struct node_allocator_t;

//———————————————————————————————————————————————————————————————————//

struct lang_ctx_t
{
    FILE*             input_file;
    FILE*             output_file;
    size_t            input_size;

    node_allocator_t* node_allocator;
    name_table_t      name_table;
    node_t*           tree;
    node_t**          nodes;
    size_t            n_nodes;
    char*             code;

    // for frontend
    stack_t           id_stack;
    stack_t           id_counter_stack;
    size_t            cur_line;
    size_t            pos;

    // for backend
    int               level;
    size_t            n_labels;
    size_t            n_globals;
    size_t            n_locals;
};

//———————————————————————————————————————————————————————————————————//

lang_status_t lang_ctx_ctor (lang_ctx_t* ctx,
                             int argc,
                             const char* argv[],
                             const char* default_input,
                             const char* default_output);

lang_status_t lang_ctx_dtor (lang_ctx_t* ctx);

//———————————————————————————————————————————————————————————————————//

#endif // LANG_H__
