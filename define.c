#include <stdlib.h>

#include "define.h"
#include "token.h"
#include "calc.h"

extern var_list *var_list_head;

define_ret var_add(char *expr, var_list **head)
{
    int pos = 0;
    var add;
    token t;
    if ((t = get_token(expr, &pos)).type != token_empty) {
        char *name = t.name;
        if (strcmp((t = get_token(expr, &pos)).name, "="))
            return def_nop;
        char *value = malloc(strlen(expr));
        strcpy(value, expr + pos);
        if (produse(value, &add)) {
            strcpy(add.name, name);
            //PUSH(*head, add);
            var_to_list(head, add);
            return def_succ;
        }
        else
            return def_err;
    } else
        return def_nop;
}

define_ret fun_add(char *expr, fun_list **head)
{
    if (!strchr(expr, '='))
        return def_nop;
    int pos = 0;
    fun add;
    add.type = fun_extended;
    int par_amount = 0;
    token t;
    t = get_token(expr, &pos);
    strcpy(add.name, t.name);
    get_token(expr, &pos); // Read '('
    add.par = NULL;
    var par_add;
    while ((t = get_token(expr, &pos)).type != token_brc_c) {
        if (t.type == token_arg_delim)
            continue;
        if (t.type != token_var) {
            CLEAR_VAR(add.par);
            for (int i = 0; i < par_amount; ++i)
                POP_VAR(var_list_head);
            return def_nop;
        }
        strcpy(par_add.name, t.name);
        par_add.type = var_double;
        PUSH(add.par, par_add);
        PUSH(var_list_head, par_add); // Add to global
        ++par_amount;
    }
    if (strcmp((t = get_token(expr, &pos)).name, "=")) {
        CLEAR_VAR(add.par);
        for (int i = 0; i < par_amount; ++i)
            POP_VAR(var_list_head); // Remove from global list
        return def_nop;
    }
    add.par_amount = par_amount;
    strcpy(add.expr, expr + pos);
    token_list *check;
    if (!(check = polish_convert(add.expr)))
        return def_err;
    for (int i = 0; i < par_amount; ++i)
        POP_VAR(var_list_head); // Remove from global list
    CLEAR_TOKEN(check);
    PUSH(*head, add);
    return def_succ;
}

define_ret check_define(char *expr, var_list **v, fun_list **f)
{
    //token_list *head = NULL;
    token t;
    int pos = 0;
    if ((t = get_token(expr, &pos)).type != token_empty) {
        if (t.type == token_var) {
            return var_add(expr, v);
        } else if (t.type == token_fun) {
            return fun_add(expr, f);
        }
    }
    return def_nop;
}
