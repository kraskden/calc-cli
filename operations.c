#include "operations.h"
#include <string.h>

void operations_init(operation_list **head)
{
    operation add;
#define ADD_OP(op, a_priority, a_type) strcpy(add.name, #op); add.priority = (a_priority); \
    add.type = (a_type); PUSH(*head, add)

    ADD_OP(+, 5, op_plus); ADD_OP(-, 5, op_minus);
    ADD_OP(*, 10, op_mult); ADD_OP(/, 10, op_div);
    ADD_OP(^, 20, op_power);
    ADD_OP(negative, 5, op_negative);
    ADD_OP(=, 1, op_equal);
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

// Right-associative operations also supported
int op_is_higher(operation_list *head, char *fst, char *snd)
{
    int fst_priority = op_get_priority(head, fst);
    int snd_priority = op_get_priority(head, snd);
    if (!strcmp(fst, "^"))
        return fst_priority > snd_priority;
    else
        return fst_priority >= snd_priority;
}

operation *name_to_operation(operation_list *head, const char *name)
{
    for (; head; head = head->next) {
        if (!strcmp(head->op.name, name))
            return &(head->op);
    }
    return NULL;
}
