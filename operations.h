#ifndef OPERATIONS_H
#define OPERATIONS_H

#define OPERATION_SIZE 255

#include "list.h"

enum operation_type {
    op_plus, op_minus, op_mult, op_div, op_power, op_negative, op_equal
};

typedef enum operation_type operation_type;

struct operation {
    char name[OPERATION_SIZE];
    int priority;
    operation_type type;
};

typedef struct operation operation;

struct operation_list {
    operation op;
    struct operation_list *next;
};

typedef struct operation_list operation_list;

#define POP_OP(head) POP(head, operation)
#define FIND_OP(head, op) FIND((head), (op), strlen((op).name))
#define CLEAR_OP(head) CLEAR(head, operation)

void operations_init(operation_list **head);
int op_get_priority(operation_list *head, char *name);
operation *name_to_operation(operation_list *head, const char *name);
#endif
