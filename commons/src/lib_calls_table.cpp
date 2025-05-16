#include "custom_assert.h"
#include "buffer.h"
#include "lib_call_funcs.h"

#include <stdint.h>
#include <string.h>

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
                                      lib_calls_table_t*      table)
{
    ASSERT(table);

    size_t table_size = table->size;

    for (size_t i = 0; i < table_size; i++) {
        lib_call_request_t* request = &table->requests[i];

        uint32_t req_addr = (uint32_t) (request->addr + 4);
        uint32_t rel      = 0;

        if        (request->type == LIB_CALL_IN) {
            rel = (uint32_t) (table->in_addr - req_addr);
        } else if (request->type == LIB_CALL_OUT) {
            rel = (uint32_t) (table->out_addr - req_addr);
        } else {
            return LANG_ERROR;
        }

        memcpy(code_buf->data + request->addr, &rel, 4);
    }

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t load_lib_func(buffer_t* bin_buf, size_t* source_addr, const char* path)
{
    ASSERT(bin_buf);
    ASSERT(source_addr);

    FILE* fp = fopen(path, "rb");
    VERIFY(!fp, return LANG_FILE_OPEN_ERROR);

    size_t source_size = 0;

    VERIFY(fread(&source_size, sizeof(size_t), 1, fp) != 1,
           return LANG_FREAD_ERROR);

    uint8_t* source_code = (uint8_t*) calloc(source_size + 1, sizeof(uint8_t));
    VERIFY(!source_code,
           return LANG_STD_ALLOCATE_ERROR);

    VERIFY(fread(source_code, sizeof(uint8_t), source_size, fp) != source_size,
           return LANG_FREAD_ERROR);

    *source_addr = bin_buf->size;

    buf_write(bin_buf, source_code, source_size);

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t load_lib_func_in(buffer_t* bin_buf, size_t* in_addr)
{
    return load_lib_func(bin_buf, in_addr, "./back-end/lib/scan_source.bin");
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t load_lib_func_out(buffer_t* bin_buf, size_t* out_addr)
{
    return load_lib_func(bin_buf, out_addr, "./back-end/lib/print_source.bin");
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t solve_lib_call_requests(lang_ctx_t* ctx)
{
    ASSERT(ctx);

    if (ctx->lib_calls_table.load_in) {
        load_lib_func_in(&ctx->bin_buf, &ctx->lib_calls_table.in_addr);
    }
    if (ctx->lib_calls_table.load_out) {
        load_lib_func_out(&ctx->bin_buf, &ctx->lib_calls_table.out_addr);
    }

    return lib_call_requests_apply(&ctx->bin_buf,
                                   &ctx->lib_calls_table);
}
