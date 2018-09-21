#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "token.h"

#include "var.h"
#include "fun.h"

#include "extmath.h"

#define DOT '.'
#define ARG_DELIMIER ';'
#define TOKEN_NAME_SIZE 255

#define OK_NAME "OK"
#define ERR_NAME "ERR"

var_list *var_list_head = NULL;
fun_list *fun_list_head = NULL;

// Need to remove this holy shit and replace it by list
const char bin_operations[] = {'+', '-', '*', '/', '^', '=', 'n'};
const int bin_operation_order[] = {5, 5, 10, 10, 20, 1, 5};

/*
 * TODO LIST:
 * 1) Remove bin_operators[] to list
 * 2) var.value transform to num structurre
 * 3)
 */

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

int is_float_const(const char *name)
{
    for (int i = 0; i < (int)strlen(name); ++i)
        if (name[i] == DOT)
            return 1;
    return 0;
}
int is_token_correct(const token *arg)
{
    int dot_count = 0;
    fun f;
    var v;
    switch (arg->type) {
    case token_const:
        for (int i = 0; i < (int)strlen(arg->name); ++i)
            dot_count += (arg->name[i] == DOT);
        return (dot_count < 2);
        break;
    case token_var:
        strcpy(v.name, arg->name);
        if (IS_EXIST_VAR(var_list_head, v))
            return 1;
        return 0;
        break;
    case token_fun:
        strcpy(f.name, arg->name);
        if (IS_EXIST_FUNC(fun_list_head, f))
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

token_list *polish_convert(char *expr)
{
    token_list *result = NULL;
    token_stack *head = NULL;
    token t;
    while ((t = get_token(expr)).type != token_empty) {
        if (!is_token_correct(&t))
            goto error;
        if (t.type == token_const || t.type == token_var) // token_var
            PUT(result, t);
        if (t.type == token_fun || t.type == token_brc_o)
            PUSH(head, t);
        if (t.type == token_brc_c) {
            token *pop_token;
            int is_correct = 0;
            while ((pop_token = POP_TOKEN(head))) {
                if ((*pop_token).type == token_brc_o) {
                    is_correct = 1;
                    break;
                }
                if ((*pop_token).type == token_fun || (*pop_token).type == token_op)
                    PUT(result, *pop_token);
            }
            if (head && head->item.type == token_fun) {
                pop_token = POP_TOKEN(head);
                PUT(result, *pop_token);
            }
            if (!is_correct)
                goto error;
        }
        if (t.type == token_op) {
            token *pop_token;
            //int is_correct = 0;
            while (head &&
                    ((*head).item.type == token_op &&
                    operation_get_priority((*head).item.name[0]) >=
                    operation_get_priority(t.name[0]))) {
                        if ((pop_token = POP_TOKEN(head))) {
                            PUT(result, *pop_token);
                            free(pop_token);
                        }
            }
            PUSH(head, t);
        }
    }
    token *pop_token;
    while ((pop_token = POP_TOKEN(head))) {
        if (pop_token->type == token_empty)
            break;
        if (pop_token->type != token_op)
            goto error;
        PUT(result, *pop_token);
    }
    CLEAR_TOKEN(head);
    return result;
error:
    CLEAR_TOKEN(head);
    CLEAR_TOKEN(result);
    return NULL;
}

int name_to_var(char *src, var *out)
{
    for (var_list *tmp = var_list_head; tmp; tmp = tmp->next) {
        if (!strcmp(src, tmp->variable.name)) {
            *out = tmp->variable;
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

int calculate(token_list *head, var *out)
{
    var_list *stack = NULL;
    var to_push;
    for (; head; head = head->next) {
        if (head->item.type == token_const) {
            expr_to_var(head->item.name, &to_push);
            PUSH(stack, to_push);
        } else if (head->item.type == token_var) {
            name_to_var(head->item.name, &to_push);
            PUSH(stack, to_push);
        } else if (head->item.type == token_op) {
            if (!strcmp(head->item.name, "negative")) {
                if (!stack)
                    return 0;
                var *op = POP_VAR(stack);
                if (op->type == var_double)
                    op->value.double_val *= -1;
                else
                    op->value.int_val *= -1;
                PUSH(stack, *op);
            } else {
                var *op1, *op2;
                if (!stack)
                    return 0;
                op2 = POP_VAR(stack); // Test!!! Maybe first op1, next - op2
                if (!stack)
                    return 0;
                op1 = POP_VAR(stack);
                var res;

#define OPERATION_APPLY_INT(operation) \
    if (!strcmp(head->item.name, #operation)) { \
        res.type = var_int; \
        res.value.int_val = op1->value.int_val operation op2->value.int_val; \
        printf("Int:: %d " #operation " %d = %d\n", op1->value.int_val, \
        op2->value.int_val, res.value.int_val); \
        PUSH(stack, res);  \
    }

#define OPERATION_APPLY_DOUBLE(operation) \
    if (!strcmp(head->item.name, #operation)) { \
        res.type = var_double; \
        res.value.double_val = op1->value.double_val operation op2->value.double_val; \
        printf("Double:: %lf " #operation " %lf = %lf\n", op1->value.double_val, \
        op2->value.double_val, res.value.double_val); \
        PUSH(stack, res); \
    }
                if (op1->type == var_int && op2->type == var_int) {
                    OPERATION_APPLY_INT(+);
                    OPERATION_APPLY_INT(-);
                    OPERATION_APPLY_INT(*);
                    if (op2->value.int_val != 0) {
                        if (op1->value.int_val % op2->value.int_val) {
                            op1->value.double_val = op1->value.int_val;
                            op2->value.double_val = op2->value.int_val;
                            OPERATION_APPLY_DOUBLE(/);
                        } else {
                            OPERATION_APPLY_INT(/);
                        }
                    } else {
                        goto fail;
                    }
                    if (!strcmp(head->item.name, "^")) {
                        res.type = var_double;
                        res.value.double_val = pow(op1->value.int_val, op2->value.int_val);
                        PUSH(stack, res);
                    }
                } else {
                    // FIX !! - OK
                    if (op1->type == var_int)
                        op1->value.double_val = op1->value.int_val;
                    if (op2->type == var_int)
                        op2->value.double_val = op2->value.int_val;
                    OPERATION_APPLY_DOUBLE(+);
                    OPERATION_APPLY_DOUBLE(-);
                    OPERATION_APPLY_DOUBLE(*);
                    if (op2->value.double_val != 0) {
                        OPERATION_APPLY_DOUBLE(/)
                    } else
                        goto fail;
                    if (!strcmp(head->item.name, "^")) {
                        res.type = var_double;
                        res.value.double_val = pow(op1->value.double_val,
                                                   op2->value.double_val);
                        PUSH(stack, res);
                    }
                }
                free(op1);
                free(op2);
            }
        } else if (head->item.type == token_fun) {
            var *op;
            if (!stack)
                goto fail;
            op = POP_VAR(stack);
            if (op->type == var_int) {
                op->value.double_val = op->value.int_val;
            }
            var res;
            res.type = var_double;

#define FUN_CALL(fun) \
    res.value.double_val = fun(op->value.double_val); \
    printf("Fun:: " #fun "(%lf) = %lf\n", op->value.double_val, res.value.double_val); \
    PUSH(stack, res);

#define FUN_APPLY(fun) \
    if (!strcmp(head->item.name, #fun)) { \
        FUN_CALL(fun); \
    }

#define FUN_APPLY_DIAPASONE(fun, left, right) \
    if (!strcmp(head->item.name, #fun)) { \
        if (op->value.double_val >= left && op->value.double_val <= right) { \
            FUN_CALL(fun); \
        } else \
            goto fail; \
    }
            FUN_APPLY(sin); FUN_APPLY(cos); FUN_APPLY(tan); FUN_APPLY(ctg);
            FUN_APPLY(exp); FUN_APPLY(atan); FUN_APPLY(actg);
            FUN_APPLY_DIAPASONE(asin, -1, 1);
            FUN_APPLY_DIAPASONE(acos, -1, 1);
            FUN_APPLY_DIAPASONE(sqrt, 0, INFINITY);
            FUN_APPLY_DIAPASONE(log, 0, INFINITY);
            FUN_APPLY_DIAPASONE(log10, 0, INFINITY);
            // Add func
            free(op);
        }
    }

    if (!stack)
        goto fail;
    var *result = POP_VAR(stack);
    *out = *result;
    if (stack)
        goto fail;
    return 1;
fail:
    CLEAR_VAR(stack);
    return 0;

#undef OPERATION_APPLY_DOUBLE
#undef OPERATION_APPLY_INT
#undef FUN_APPLY
#undef FUN_APPLY_DIAPASONE
#undef FUN_CALL
}

int main()
{
    var foo;
    for (int i = 0; i < 10; ++i) {
        sprintf(foo.name, "var%d", i);
        PUSH(var_list_head, foo);
    }
    fun_init_base(&fun_list_head);
    char *expr = malloc(TOKEN_NAME_SIZE * sizeof (*expr));
    fgets(expr, TOKEN_NAME_SIZE, stdin);
    if (*expr)
        expr[strlen(expr) - 1] = '\0';
    token_list *res;
    if ((res = polish_convert(expr))) {
        for (token_list *tmp = res; tmp; tmp = tmp->next) {
            printf("%s ", tmp->item.name);
        }
        putchar('\n');
    } else {
        puts("An error occupped!\n");
    }
    var answer;
    if (calculate(res, &answer)) {
        if (answer.type == var_double) {
            printf("Double: %lf\n", answer.value.double_val);
        } else {
            printf("Int: %d\n", answer.value.int_val);
        }
    } else {
        puts("An error calc occuped!\n");
    }
    CLEAR_TOKEN(res);
    CLEAR_FUNC(fun_list_head);
    CLEAR_VAR(var_list_head);
    return 0;
}
