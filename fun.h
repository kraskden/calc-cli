#ifndef FUN_H
#define FUN_H

#define FUNC_NAME_SIZE 255
#define EXPR_SIZE 255

#include "list.h"

enum fun_type {fun_base, fun_extended};

typedef enum fun_type fun_type;

struct fun {
    char name[FUNC_NAME_SIZE];
    fun_type type;
    char expr[EXPR_SIZE];
};

typedef struct fun fun;

struct fun_list {
    fun function;
    struct fun_list *next;
};

typedef struct fun_list fun_list;

#define POP_FUNC(head) POP(head, fun)
#define IS_EXIST_FUNC(head, fun) IS_EXIST(head, fun, FUNC_NAME_SIZE)
#define CLEAR_FUNC(head) CLEAR(head, fun)

//int add_fun(fun arg, fun_list **head);
//fun* get_fun(const char *name, fun_list *head);
//void free_fun(fun_list **head);
//void fun_init_base(fun_list **head);

#endif
