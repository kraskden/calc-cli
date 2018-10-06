#include "var.h"
#include "extmath.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>

void var_init(var_list **head)
{
    var add;
    add.type = var_double;
#define ADD_V(NAME, VAL) strcpy(add.name, (NAME)); add.value.double_val = (VAL); \
    PUSH(*head, add);
    ADD_V("pi", M_PI); ADD_V("e", M_E); ADD_V("ln10", M_LN10);
    ADD_V("ln2", M_LN2); ADD_V("sqrt2", M_SQRT2); ADD_V("log10e", M_LOG10E);
    ADD_V("log2e", M_LOG2E); ADD_V("pi2", M_PI_2);
#undef ADD_V
}

int name_to_var(var_list *head, char *src, var *out)
{
    for (; head; head = head->next) {
        if (!strcmp(src, head->variable.name)) {
            *out = head->variable;
            return 1;
        }
    }
    return 0;
}

void expr_to_var(char *src, var *out)
{
    strcpy(out->name, src);
    if (strchr(src, DOT)) {
        out->type = var_double;
        out->value.double_val = atof(src);
    } else {
        out->type = var_int;
        out->value.int_val = atoi(src);
    }
}

void var_to_list(var_list **head, var arg)
{
    int is_exist = 0;
    var_list *tmp;
    for (tmp = *head; tmp; tmp = tmp->next) {
        if ((is_exist = !strcmp(tmp->variable.name, arg.name)))
            break;
    }
    if (is_exist) {
        tmp->variable = arg;
    } else
        PUSH(*head, arg);
}

int is_float_const(const char *name)
{
    for (int i = 0; i < (int)strlen(name); ++i)
        if (name[i] == DOT)
            return 1;
    return 0;
}
