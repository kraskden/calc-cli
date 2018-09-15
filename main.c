#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "define.h"

//#include "var.h"
//#include "fun.h"

#define DOT '.'
#define ARG_DELIMIER ';'
#define TOKEN_NAME_SIZE 255

var_list *var_list_head = NULL;
fun_list *fun_list_head = NULL;

const char bin_operations[] = {'+', '-', '*', '/', '^', '=', 'n'};
const int bin_operation_order[] = {5, 5, 10, 10, 20, 1, 5};

int operation_get_priority(char op)
{
    for (int i = 0; i < (int)sizeof (bin_operations); ++i)
        if (bin_operations[i] == op)
            return bin_operation_order[i];
    return 0;
}

int is_operation_correct(char *name)
{
    if (strlen(name) == 1) {
        int ret = 0;
        for (int i = 0; i < (int)sizeof (bin_operations); ++i)
            if (*name == bin_operations[i])
                ret = 1;
        return ret;
    } else if (!strcmp(name, "negative"))
        return 1;
    else
        return 0;
}

void skip_spaces(char *expr, int *pos)
{
    int i;
    for (i = *pos; i < (int)strlen(expr); ++i)
        if (expr[i] != ' ' && expr[i] != '\t')
            break;
    *pos = i;
}

void print_type(token_type type)
{
    switch (type) {
    case token_const:
        printf(" :: Const");
        break;
    case token_fun:
        printf(" :: Function");
        break;
    case token_var:
        printf(" :: Variable");
        break;
    case token_op:
        printf(" :: Operator");
        break;
    default:
        printf(" :: Other");
    }
}

int is_token_correct(const token *arg)
{
    int dot_count = 0;
    switch (arg->type) {
    case token_const:
        for (int i = 0; i < (int)strlen(arg->name); ++i)
            dot_count += (arg->name[i] == DOT);
        return (dot_count < 2);
        break;
    case token_var:
        if (get_var(arg->name, var_list_head))
            return 1;
        return 0;
        break;
    case token_fun:
        if (get_fun(arg->name, fun_list_head))
            return 1;
        return 0;
        break;
    case token_op:
        return  is_operation_correct(arg->name);
        return 0;
        break;
    default:
        return 1;
        break;
    }
}

token get_token(char *expr)
{
    token ret;
    ret.type = token_empty;
    if (!*expr)
        return ret;
    static int pos = 0;
    char ch;
    int  prev_pos = pos - 1;
    char* token_name = (char*) malloc(TOKEN_NAME_SIZE * sizeof (*token_name));
    skip_spaces(expr, &pos);
    if ((ch = expr[pos++])) {
        *token_name = ch;
        token_name[1] = '\0';
        if (ch == '(')
            ret.type = token_brc_o;
        else if (ch == ')')
            ret.type = token_brc_c;
        else if (ch == ARG_DELIMIER)
            ret.type = token_arg_delim;
        else if (ch == '-' && (pos == 1 ||
        (!isalpha(expr[prev_pos]) && !isdigit(expr[prev_pos]) && expr[prev_pos] != ')'))) {
            //ret.type = token_fun;
            ret.type = token_op;
            strcpy(token_name, "negative");
        } else
            ret.type = token_op;
        if (isdigit(ch) || (ch == DOT)) {
            ret.type = token_const;
            int i;
            for (i = 1; (ch = expr[pos]); ) {
                if (!isdigit(ch) && (ch != DOT)) {
                    break;
                } else {
                    pos++;
                    token_name[i++] = ch;
                }
            }
            token_name[i] = '\0';
        } else if (isalpha(ch) || ch == '_') {
            ret.type = token_var;
            int i;
            for (i = 1; (ch = expr[pos]); ) {
                if (ch == '(') {
                    ret.type = token_fun;
                    break;
                }
                if (isalpha(ch) || isdigit(ch) || ch == '_') {
                    pos++;
                    token_name[i++] = ch;
                } else
                    break;
            }
        }
        ret.name = (char*) malloc((strlen(token_name) + 1) * sizeof (char));
        strcpy(ret.name, token_name);
    }
    free(token_name);
    return ret;
}

char* polish_convert(char *expr)
{
    token_stack *head = NULL;
    token t;
    char *ret = (char*) malloc((strlen(expr) + 1) * sizeof (char));
    *ret = 0;
    while ((t = get_token(expr)).type != token_empty) {
        if (!is_token_correct(&t))
            goto error;
        if (t.type == token_const)
            strcat(ret, t.name);
        if (t.type == token_fun || t.type == token_brc_o)
            token_stack_push(&head, &t);
        if (t.type == token_brc_c) {
            token *pop;
            int is_correct = 0;
            while ((pop = token_stack_pop(&head))) {
                if ((*pop).type == token_brc_o) {
                    is_correct = 1;
                    break;
                }
                if ((*pop).type == token_fun || (*pop).type == token_op)
                    strcat(ret, (*pop).name);
            }
            if (head->item.type == token_fun) {
                pop = token_stack_pop(&head);
                strcat(ret, (*pop).name);
            }
            if (!is_correct)
                goto error;
        }
        if (t.type == token_op) {
            token *pop;
            //int is_correct = 0;
            while (head && /*(*head).item.type == token_fun || */
                    ((*head).item.type == token_op &&
                    operation_get_priority((*head).item.name[0]) >=
                    operation_get_priority(t.name[0]))) {
                        if ((pop = token_stack_pop(&head)))
                            strcat(ret, (*pop).name);
            }
            token_stack_push(&head, &t);
        }
    }
    token *pop;
    while ((pop = token_stack_pop(&head))) {
        if (pop->type == token_empty)
            break;
        if ((*pop).type != token_op)
            goto error;
        strcat(ret, (*pop).name);
    }
    token_stack_free(&head);
    return ret;
error:
    token_stack_free(&head);
    free(ret);
    return NULL;
}

int main()
{
    var foo;
    for (int i = 0; i < 10; ++i) {
        sprintf(foo.name, "var%d", i);
        add_var(foo, &var_list_head);
    }
    fun_init_base(&fun_list_head);
    char *expr = malloc(TOKEN_NAME_SIZE * sizeof (*expr));
    fgets(expr, TOKEN_NAME_SIZE, stdin);
    if (*expr)
        expr[strlen(expr) - 1] = '\0';
    //while ((print = get_token(expr)).type != token_empty) {
    //    printf("%s", print.name);
    //    print_type(print.type);
    //    printf(" :: %s", is_token_correct(&print) ? "true" : "false");
    //    putchar('\n');
    //}
    char *res;
    if ((res = polish_convert(expr))) {
        puts(res);
    } else {
        puts("An error occupped!\n");
    }
    free_var(&var_list_head);
    free_fun(&fun_list_head);
    return 0;
}
