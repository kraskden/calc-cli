#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "calc.h"
#include "define.h"
#include "token.h"

#include "var.h"
#include "fun.h"
#include "extmath.h"
#include "operations.h"

var_list *var_list_head = NULL;
fun_list *fun_list_head = NULL;
operation_list *op_list_head = NULL;

/*
 * TODO LIST:
 * 1) Remove bin_operators[] to list - OK
 * 2) var.value transform to num structurre
 * 3) Delim custom and build-in variable (such as pi, e, ect)
 * 4) Delim custom and build-in function
 */

int main()
{
    fun_init_base(&fun_list_head);
    var_init(&var_list_head);
    operations_init(&op_list_head);
    char *expr = malloc(TOKEN_NAME_SIZE * sizeof (*expr));
    while (1) {
        fgets(expr, TOKEN_NAME_SIZE, stdin);
        if (*expr)
            expr[strlen(expr) - 1] = '\0';
        if (!strcmp(expr, "exit"))
            break;
        //switch (var_add(expr, &var_list_head)) {
        switch (check_define(expr, &var_list_head, &fun_list_head))  {
        case def_nop:
            break;
        case def_succ:
            continue;
        case def_err:
            puts("Error var defined!");
            continue;
        }
        var answer;
        if (produse(expr, &answer)) {
            if (answer.type == var_double) {
                printf("Double: %lf\n", answer.value.double_val);
            } else {
                printf("Int: %d\n", answer.value.int_val);
            }
            strcpy(answer.name, "ans");
            var_to_list(&var_list_head, answer);
        } else {
            printf("Error\n");
        }
    }
    CLEAR_FUNC(fun_list_head);
    CLEAR_VAR(var_list_head);
    CLEAR_OP(op_list_head);
    return 0;
}
