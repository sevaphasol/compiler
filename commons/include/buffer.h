//——————————————————————————————————————————————————————————————————————————————

#ifndef _BUFFER_H__
#define _BUFFER_H__

//——————————————————————————————————————————————————————————————————————————————

#include "lang_status.h"

#include <stdio.h>
#include <inttypes.h>

//——————————————————————————————————————————————————————————————————————————————

struct buffer_t {
    uint8_t* data;
    size_t   capacity;
    size_t   size;
};

//——————————————————————————————————————————————————————————————————————————————

lang_status_t buf_ctor    (buffer_t* buf, size_t init_capacity);
lang_status_t buf_dtor    (buffer_t* buf);
lang_status_t buf_write   (buffer_t* buf, const void* data, size_t data_size);
lang_status_t buf_rewrite (buffer_t* buf, const void* data, size_t data_size, size_t offset);

//——————————————————————————————————————————————————————————————————————————————

#endif // _BUFFER_H__

//——————————————————————————————————————————————————————————————————————————————
