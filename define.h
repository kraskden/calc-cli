#ifndef DEFINE_H
#define DEFINE_H

#include "fun.h"
#include "var.h"

enum define_ret {
    def_var, def_fun, def_nop, def_err
};

typedef enum define_ret define_ret;
define_ret check_define(char *expr, var_list **v, fun_list **f);

#endif
