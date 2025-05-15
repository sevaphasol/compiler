#include "lib_calls_table.h"
#include "custom_assert.h"
#include "buffer.h"

#include <stdint.h>

//——————————————————————————————————————————————————————————————————————————————

lang_status_t lib_calls_table_ctor(lib_calls_table_t* table,
                                   size_t             init_capacity)
{
    ASSERT(table);

    table->requests = (lib_call_request_t*) calloc(init_capacity,
                                                   sizeof(lib_call_request_t));

    if (!table->requests) {
        return LANG_ERROR;
    }

    table->capacity = init_capacity;
    table->size = 0;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t lib_calls_table_dtor(lib_calls_table_t* table)
{
    ASSERT(table);

    if (!table->requests) {
        return LANG_ERROR;
    }

    free(table->requests);

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t add_lib_call_request(lib_calls_table_t*      table,
                                   lib_call_request_type_t type,
                                   size_t                  addr)
{
    ASSERT(table);
    ASSERT(type != LIB_CALL_INVALID);

    if (table->size >= table->capacity) {
        table->requests = (lib_call_request_t*) realloc(table->requests,
                                                        table->capacity *
                                                        2 * sizeof(lib_call_request_t));
        if (!table->requests) {
            return LANG_ERROR;
        }

        table->capacity *= 2;
    }

    table->requests[table->size++] = {
        .type = type,
        .addr = addr
    };

    if         (type == LIB_CALL_IN  && !table->load_in) {
        table->load_in = true;
    } else if  (type == LIB_CALL_OUT && !table->load_out) {
        table->load_out = true;
    }

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t lib_call_requests_apply(buffer_t*               code_buf,
                                      lib_calls_table_t*      table,
                                      lib_call_request_type_t type,
                                      size_t                  in_addr,
                                      size_t                  out_addr)
{
    ASSERT(table);

    size_t table_size = table->size;

    for (size_t i = 0; i < table_size; i++) {
        lib_call_request_t* request = &table->requests[i];

        uint32_t req_addr = request->addr;
        uint32_t rel      = 0;

        if        (request->type == LIB_CALL_IN) {
            rel = req_addr - in_addr;
        } else if (request->type == LIB_CALL_OUT) {
            rel = req_addr - out_addr;
        } else {
            return LANG_ERROR;
        }

        memcpy(code_buf->data + request->addr, &rel, 4);
    }

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————
