#include <mparser.h>
#include <calc.h>
#include <var.h>
#include <fun.h>
#include <operations.h>
#include <define.h>

var_list *var_list_head = NULL;
fun_list *fun_list_head = NULL;
operation_list *op_list_head = NULL;

void mparser_init()
{
    fun_init_base(&fun_list_head);
    var_init(&var_list_head);
    operations_init(&op_list_head);
}

void mparser_destroy()
{
    CLEAR_FUNC(fun_list_head);
    CLEAR_VAR(var_list_head);
    CLEAR_OP(op_list_head);
}

parses_ret mparser_parse(const char *expr, var *out)
{
    switch (check_define(expr, &var_list_head, &fun_list_head)) {
    case def_nop:
        break;
    case def_err:
        return prs_err;
    case def_var:
        return prs_def_var;
    case def_fun:
        return prs_def_fun;
    }
    if (produse(expr, out)) {
        var ans;
        strcpy(ans.name, "ans");
        ans.type = out->type;
        ans.value = out->value;
        var_to_list(&var_list_head, ans);
        return prs_produse;
    } else
        return prs_err;
}
