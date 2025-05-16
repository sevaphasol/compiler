//——————————————————————————————————————————————————————————————————————————————

#ifndef _LIB_CALL_FUNCS_H__
#define _LIB_CALL_FUNCS_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang.h"
#include "buffer.h"
#include "lib_calls_table.h"

#include <stddef.h>

//——————————————————————————————————————————————————————————————————————————————

lang_status_t lib_calls_table_ctor(lib_calls_table_t*      table,
                                   size_t                  init_capacity);
lang_status_t lib_calls_table_dtor(lib_calls_table_t*      table);
lang_status_t add_lib_call_request(lib_calls_table_t*      table,
                                   lib_call_request_type_t type,
                                   size_t                  addr);
lang_status_t lib_call_requests_apply(buffer_t*            code_buf,
                                      lib_calls_table_t*   table);
lang_status_t solve_lib_call_requests(lang_ctx_t* ctx);

//——————————————————————————————————————————————————————————————————————————————

#endif // _LIB_CALL_FUNCS_H__

//——————————————————————————————————————————————————————————————————————————————
