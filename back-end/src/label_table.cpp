#include "lang_status.h"
#include "fixup_table.h"
#include <stdlib.h>
#include <string.h>

//——————————————————————————————————————————————————————————————————————————————

lang_status_t label_table_ctor(label_table_t* table, size_t init_capacity)
{
    ASSERT(table);

    table->labels = calloc(init_capacity, sizeof(label_t));

    if (!table->labels) {
        return LANG_STD_ALLOCATE_ERROR;
    }

    table->capacity = init_capacity;
    table->size     = 0;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t label_table_dtor(label_table_t* table)
{
    ASSERT(table);

    for (size_t i = 0; i < table->size; ++i) {
        if (table->labels[i].is_global &&
            table->labels[i].info.global_name) {
            free(table->labels[i].info.global_name); // for strdup
        }
    }

    free(table->labels);

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t label_table_expand(label_table_t* table)
{
    ASSERT(table);

    table->labels = (label_t*) realloc(table->labels, table->capacity * sizeof(label_t));

    if (!table->labels) {
        return LANG_STD_ALLOCATE_ERROR;
    }

    table->capacity *= 2;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t label_table_add_global(label_table_t* table, const char* name, size_t address)
{
    ASSERT(table);

    if (table->count >= table->capacity) {
        if (!label_table_expand(table)) return LANG_STD_ALLOCATE_ERROR;
    }

    table->labels[table->size++] = {
        .address = address,
        .info.global_name = strdup(name),
        .is_global = 1
    };

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t label_table_add_local(label_table_t* table, size_t number, size_t address)
{
    ASSERT(table);

    if (table->count >= table->capacity) {
        if(!label_table_expand(table)) return LANG_STD_ALLOCATE_ERROR;
    }

    table->labels[table->size++] = {
        .address = address,
        .info.local_number = number,
        .is_global = 0
    };

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t label_table_find_global(label_table_t* table, const char* name, size_t* addr)
{
    ASSERT(table);
    ASSERT(name);

    for (size_t i = 0; i < table->size; ++i) { //TODO hashmap
        if (table->labels[i].is_global &&
            strcmp(table->labels[i].info.global_name, name) == 0) {
            *size = table->labels[i].address;
            return LANG_SUCCESS;
        }
    }

    return LANG_ERROR;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t label_table_find_local(label_table_t* table, size_t number, size_t* addr)
{
    ASSERT(table);
    ASSERT(addr);

    for (size_t i = 0; i < table->count; ++i) {
        if (!table->labels[i].is_global &&
            table->labels[i].info.local_number == number) {
            *addr = table->labels[i].address;
            return LANG_SUCCESS;
        }
    }

    return LANG_ERROR;
}

//——————————————————————————————————————————————————————————————————————————————
