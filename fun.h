#ifndef FUN_H
#define FUN_H

#define FUNC_NAME_SIZE 255
#define EXPR_SIZE 255

#include "list.h"
#include "var.h"
#include <string.h>

enum fun_type {fun_base, fun_extended};

typedef enum fun_type fun_type;

struct fun {
    char name[FUNC_NAME_SIZE];
    fun_type type;
    int par_amount;
    var_list *par;
    char expr[EXPR_SIZE];
};

typedef struct fun fun;

struct fun_list {
    fun function;
    struct fun_list *next;
};

typedef struct fun_list fun_list;

#define POP_FUNC(head) POP(head, fun)
#define FIND_FUN(head, fun) FIND(head, (fun), strlen((fun).name))
#define CLEAR_FUNC(head) fun_var_clear(head); CLEAR(head, fun)

//int add_fun(fun arg, fun_list **head);
//fun* get_fun(const char *name, fun_list *head);
//void free_fun(fun_list **head);
//void fun_init_base(fun_list **head);

void fun_init_base(fun_list **head);
void fun_var_clear(fun_list *head);
const fun* name_to_fun(const fun_list *head, const char* name);
#endif
