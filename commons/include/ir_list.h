#ifndef _IR_LIST_H__
#define _IR_LIST_H__

//——————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "ir_instr.h"
#include "lang_status.h"

//——————————————————————————————————————————————————————————————————————————————

struct ir_node_t {
    ir_instr_t instr;
    ir_node_t* next;
    ir_node_t* prev;
};

//——————————————————————————————————————————————————————————————————————————————

ir_node_t* new_ir_node();

lang_status_t cut_node    (ir_node_t* ir_node);
lang_status_t ir_list_dtor(ir_node_t* ir_list);

//——————————————————————————————————————————————————————————————————————————————

#endif // _IR_LIST_H__
