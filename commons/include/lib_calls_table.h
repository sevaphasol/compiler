//——————————————————————————————————————————————————————————————————————————————

#ifndef _LIB_CALLS_TABLE_H__
#define _LIB_CALLS_TABLE_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang_status.h"

#include <stddef.h>

//——————————————————————————————————————————————————————————————————————————————

enum lib_call_request_type_t {
    LIB_CALL_INVALID = 0,
    LIB_CALL_IN      = 1,
    LIB_CALL_OUT     = 2,
};

//——————————————————————————————————————————————————————————————————————————————

struct lib_call_request_t {
    lib_call_request_type_t type;
    size_t                  addr;
};

//——————————————————————————————————————————————————————————————————————————————

struct lib_calls_table_t {
    lib_call_request_t* requests;
    size_t              capacity;
    size_t              size;

    size_t              in_addr;
    size_t              out_addt;
    bool                load_in;
    bool                load_out;
};

//——————————————————————————————————————————————————————————————————————————————

lang_status_t lib_calls_table_ctor(lib_calls_table_t*      table,
                                   size_t                  init_capacity);
lang_status_t lib_calls_table_dtor(lib_calls_table_t*      table);
lang_status_t add_lib_call_request(lib_calls_table_t*      table,
                                   lib_call_request_type_t type,
                                   size_t                  addr);

//——————————————————————————————————————————————————————————————————————————————

#endif // _LIB_CALLS_TABLE_H__

//——————————————————————————————————————————————————————————————————————————————
