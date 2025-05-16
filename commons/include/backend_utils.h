#ifndef _BACKEND_UTILS_H__
#define _BACKEND_UTILS_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang.h"

//——————————————————————————————————————————————————————————————————————————————

lang_status_t backend_lang_ctx_ctor   (lang_ctx_t* ctx,
                                       int         argc,
                                       char*       argv[]);

lang_status_t backend_lang_ctx_dtor   (lang_ctx_t* ctx);

lang_status_t read_tree               (lang_ctx_t* ctx,
                                       node_t**    node);

lang_status_t put_node_value          (int         type,
                                       int         val,
                                       value_t*    node_value);

lang_status_t read_name_table         (lang_ctx_t* ctx);

lang_status_t read_input_ctx          (lang_ctx_t* ctx);

//——————————————————————————————————————————————————————————————————————————————

#endif // _BACKEND_UTILS_H__
