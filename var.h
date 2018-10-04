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
    struct value {
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
#define FIND_VAR(head, var) FIND(head, (var), strlen((var).name))
#define CLEAR_VAR(head) CLEAR(head, var)

//int add_var(var arg, var_list **head);
//var* get_var(const char *name, var_list *head);
//void free_var(var_list **head);

void var_init(var_list **head);
int name_to_var(var_list *head, char *src, var *out);
void expr_to_var(char *src, var *out);
void var_to_list(var_list **head, var arg);

#endif
