#include "ir_list.h"
#include "lang_status.h"
#include "custom_assert.h"

//——————————————————————————————————————————————————————————————————————————————

ir_node_t* new_ir_node()
{
    return (ir_node_t*) calloc(1, sizeof(ir_node_t));
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t cut_node(ir_node_t* node)
{
    ASSERT(node);

    if(node->prev) {
        node->prev->next = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    }

    free(node);

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————

lang_status_t ir_list_dtor(ir_node_t* ir_list)
{
    ASSERT(ir_list);

    ir_node_t* cur_node = ir_list;

    while (cur_node) {
        ir_node_t* next_node = cur_node->next;
        free(cur_node);
        cur_node = next_node;
    }

    return LANG_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————
