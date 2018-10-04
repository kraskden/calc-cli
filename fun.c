#include "fun.h"

void fun_init_base(fun_list **head)
{
    fun add;
    add.type = fun_base;
    add.par_amount = 1;
#define ADD_F(NAME) strcpy(add.name, (NAME)); PUSH(*head, add)
    ADD_F("sin"); ADD_F("cos"); ADD_F("tan"); ADD_F("ctg");
    ADD_F("asin"); ADD_F("acos"); ADD_F("atan"); ADD_F("actg");
    ADD_F("exp"); ADD_F("ln"); ADD_F("log10"); ADD_F("sqrt");
    add.par_amount = 2;
    ADD_F("logbase");
    //ADD_F("negative");
#undef ADD_F
}

const fun* name_to_fun(const fun_list *head, const char* name)
{
    for (; head; head = head->next) {
        if (!strcmp(name, head->function.name)) {
            return &(head->function);
        }
    }
    return NULL;
}
