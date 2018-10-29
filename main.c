#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

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

int is_token_correct(const token *arg)
{
    int dot_count = 0;
    fun f;
    var v;
    operation o;
    switch (arg->type) {
    case token_const:
        for (int i = 0; i < (int)strlen(arg->name); ++i)
            dot_count += (arg->name[i] == DOT);
        return (dot_count < 2);
        break;
    case token_var:
        strcpy(v.name, arg->name);
        if (FIND_VAR(var_list_head, v))
            return 1;
        return 0;
        break;
    case token_fun:
        strcpy(f.name, arg->name);
        if (FIND_FUN(fun_list_head, f))
            return 1;
        return 0;
        break;
    case token_op:
        strcpy(o.name, arg->name);
        if (FIND_OP(op_list_head, o))
            return 1;
        return 0;
        break;
    default:
        return 1;
        break;
    }
}

token_list *polish_convert(char *expr)
{
    token_list *result = NULL;
    token_stack *head = NULL;
    token t;
    int pos = 0;
    while ((t = get_token(expr, &pos)).type != token_empty) {
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
            while (head &&
                    ((*head).item.type == token_op &&
                    op_get_priority(op_list_head, (*head).item.name) >=
                    op_get_priority(op_list_head, t.name))) {
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

int calculate(token_list *head, var *out)
{
    var_list *stack = NULL;
    var to_push;
    for (; head; head = head->next) {
        if (head->item.type == token_const) {
            expr_to_var(head->item.name, &to_push);
            PUSH(stack, to_push);
        } else if (head->item.type == token_var) {
            name_to_var(var_list_head, head->item.name, &to_push);
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
                    if (!strcmp(head->item.name, "/")) {
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
                    }
                    if (!strcmp(head->item.name, "^")) {
                        res.type = var_int;
                        res.value.int_val = binpow(op1->value.int_val, op2->value.int_val);
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
            const fun *prod = name_to_fun(fun_list_head, head->item.name);
            if (!prod)
                goto fail;
            var **op = malloc(sizeof(*op) * prod->par_amount);
            int i;
            for (i = 0; i < prod->par_amount && stack; ++i) {
                op[i] = POP_VAR(stack);
                if (op[i]->type == var_int) {
                    op[i]->value.double_val = op[i]->value.int_val;
                }
            }
            if (!stack && i < prod->par_amount)
                goto fail;
            //op = POP_VAR(stack);
            //if (op->type == var_int) {
            //    op->value.double_val = op->value.int_val;
            //}
            var res;
            res.type = var_double;

#define STDFUN_CALL(fun) \
    res.value.double_val = fun(op[0]->value.double_val); \
    printf("Fun:: " #fun "(%lf) = %lf\n", op[0]->value.double_val, res.value.double_val); \
    PUSH(stack, res);

#define STDFUN_APPLY(fun) \
    if (!strcmp(head->item.name, #fun)) { \
        STDFUN_CALL(fun); \
    }

#define STDFUN_APPLY_DIAPASONE(fun, left, right) \
    if (!strcmp(head->item.name, #fun)) { \
        if (op[0]->value.double_val >= left && op[0]->value.double_val <= right) { \
            STDFUN_CALL(fun); \
        } else \
            goto fail; \
    }

#define EXTFUN_APPLY(fun) \
    if (!strcmp(head->item.name, #fun)) { \
        if (!fun(op, prod->par_amount, &res)) { \
            goto fail; \
        } \
        PUSH(stack, res); \
    }

            STDFUN_APPLY(sin); STDFUN_APPLY(cos); STDFUN_APPLY(tan); STDFUN_APPLY(ctg);
            STDFUN_APPLY(exp); STDFUN_APPLY(atan); STDFUN_APPLY(actg);
            STDFUN_APPLY_DIAPASONE(asin, -1, 1);
            STDFUN_APPLY_DIAPASONE(acos, -1, 1);
            STDFUN_APPLY_DIAPASONE(sqrt, 0, INFINITY);
            STDFUN_APPLY_DIAPASONE(ln, 0, INFINITY);
            STDFUN_APPLY_DIAPASONE(log10, 0, INFINITY);
            // Add func
            EXTFUN_APPLY(logbase);
            for (int i = 0; i < prod->par_amount; ++i) {
                free(op[i]);
            }
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
#undef STDFUN_APPLY
#undef STDFUN_APPLY_DIAPASONE
#undef STDFUN_CALL
#undef EXTFUN_APPLY
}

int produse(char *expr, var *out)
{
    token_list *res;
    if (!(res = polish_convert(expr))) {
        return 0;
    }
    if (!calculate(res, out))
        return 0;
    CLEAR_TOKEN(res);
    return 1;
}


enum define_ret {
    def_succ, def_nop, def_err
};

typedef enum define_ret define_ret;

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

/*
 *
 * Test add of preprocessor
 */


define_ret check_define(char *expr, var_list **v, fun_list **f)
{
    //token_list *head = NULL;
    token t;
    int pos = 0;
    if ((t = get_token(expr, &pos)).type != token_empty) {
        if (t.type == token_var) {
            return var_add(expr, v);
        } else if (t.type == token_fun) {

        }
    }
    return def_nop;
}

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
            strcpy(answer.name, "ans");
            //PUSH(var_list_head, answer);
            var_to_list(&var_list_head, answer);
        } else {
            puts("An error calc occuped!\n");
        }
        CLEAR_TOKEN(res);
    }
    CLEAR_FUNC(fun_list_head);
    CLEAR_VAR(var_list_head);
    CLEAR_OP(op_list_head);
    return 0;
}
