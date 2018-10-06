#include "operations.h"
#include <string.h>

void operations_init(operation_list **head)
{
    operation add;
#define ADD_OP(op, a_priority) strcpy(add.name, #op); add.priority = (a_priority); \
    PUSH(*head, add);

    ADD_OP(+, 5); ADD_OP(-, 5);
    ADD_OP(*, 10); ADD_OP(/, 10);
    ADD_OP(^, 20);
    ADD_OP(negative, 5);
    ADD_OP(=, 1);
}

int op_get_priority(operation_list *head, char *name)
{
    operation *op;
    operation find_op;
    strcpy(find_op.name, name);
    if ((op = FIND_OP(head, find_op))) {
        return op->priority;
    } else
        return 0;
}
