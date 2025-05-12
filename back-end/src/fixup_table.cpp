#include "lang.h"
#include "custom_assert.h"

#include <string.h>

//——————————————————————————————————————————————————————————————————————————————

lang_status_t fixup_table_ctor(fixup_table_t* fixups, size_t init_capacity)
{
    ASSERT(fixups);

    fixups->entries = (fixup_entry_t*) calloc(init_capacity, sizeof(fixup_entry_t));

    if (!fixups->entries) {
        return LANG_ERROR;
    }

    fixups->capacity = init_capacity;
    fixups->size = 0;

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t fixup_table_dtor(fixup_table_t* fixups)
{
    ASSERT(fixups);

    if (!fixups->entries) {
        return LANG_ERROR;
    }

    free(fixups->entries);

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t add_fixup(fixup_table_t* table,
                        const char* global_name,
                        size_t      local_number,
                        uint32_t    offset)
{
    ASSERT(table);

    if (table->size >= table->capacity) {
        table->entries = (fixup_entry_t*) realloc(table->entries, table->capacity *
                                                           2 * sizeof(fixup_entry_t));
        if (!table->entries) {
            return LANG_ERROR;
        }

        table->capacity *= 2;
    }

    label_t label = {};
    if (global_name) {
        label.value.global_name = strdup(global_name);
        label.is_global = 1;
    } else {
        label.value.local_number = local_number;
        label.is_global = 0;
    }

    table->entries[table->size++] = {
        .offset = offset,
        .label = label
    };

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t fixup_table_apply(fixup_table_t* fixup_table, label_table_t* label_table, buffer_t* code_buf)
{
    ASSERT(fixup_table);
    ASSERT(label_table);

    for (size_t i = 0; i < fixup_table->size; ++i) {
        fixup_entry_t* entry = &fixup_table->entries[i];

        uint32_t target_addr = 0;
        if (entry->label.is_global) {
            VERIFY(!label_table_find_global(label_table, entry->label.value.global_name, &target_addr),
                   return LANG_ERROR);
        } else {
            VERIFY(label_table_find_local(label_table, entry->label.value.local_number, &target_addr),
                   return LANG_ERROR);
        }

        uint32_t current_addr = entry->offset + 4;
        int32_t rel = target_addr - current_addr;
        memcpy(code_buf->data + entry->offset, &rel, 4);
    }

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————
