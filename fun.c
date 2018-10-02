#include "fun.h"

void fun_init_base(fun_list **head)
{
    fun add;
    add.type = fun_base;
#define ADD_F(NAME) strcpy(add.name, (NAME)); PUSH(*head, add)
    ADD_F("sin"); ADD_F("cos"); ADD_F("tan"); ADD_F("ctg");
    ADD_F("asin"); ADD_F("acos"); ADD_F("atan"); ADD_F("actg");
    ADD_F("exp"); ADD_F("log"); ADD_F("log10"); ADD_F("sqrt");
    //ADD_F("negative");
#undef ADD_F
}
