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
#include "mparser.h"

extern var_list *var_list_head;
extern fun_list *fun_list_head;
extern operation_list *op_list_head;

int main()
{
    mparser_init();
    char *expr = malloc(TOKEN_NAME_SIZE * sizeof (*expr));
    while (1) {
        fgets(expr, TOKEN_NAME_SIZE, stdin);
        if (*expr)
            expr[strlen(expr) - 1] = '\0';
        if (!strcmp(expr, "exit"))
            break;
        //switch (var_add(expr, &var_list_head)) {
        var ans;
        switch (mparser_parse(expr, &ans)) {
        case prs_err:
            printf("Error\n"); break;
        case prs_produse: {
            if (ans.type == var_double)
                printf("Double :: %lf\n", ans.value.double_val);
            else
                printf("Int :: %d\n", ans.value.int_val);
        }
        default:
            break;
        }
    }
    mparser_destroy();
    return 0;
}
