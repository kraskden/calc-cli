#ifndef VAR_H
#define VAR_H

#define VAR_LENGTH 255

#include "list.h"

enum var_type {
    var_int, var_double
};

typedef enum var_type var_type;

struct var {
    char name[VAR_LENGTH];
    var_type type;
    union value {
        int int_val;
        double double_val;
    } value;
};

typedef struct var var;

struct var_list{
    var variable;
    struct var_list *next;
};

typedef struct var_list var_list;

#define POP_VAR(head) POP(head, var)
#define IS_EXIST_VAR(head, var) IS_EXIST(head, var, VAR_LENGTH)
#define CLEAR_VAR(head) CLEAR(head, var)

//int add_var(var arg, var_list **head);
//var* get_var(const char *name, var_list *head);
//void free_var(var_list **head);

#endif
