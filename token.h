#ifndef TOKEN_H
#define TOKEN_H

#include "list.h"

#define TOKEN_NAME_SIZE 255
#define ARG_DELIMIER ';'

enum token_type {
    token_const, token_var, token_op, token_brc_o,
    token_brc_c, token_fun, token_arg_delim, token_empty
};

typedef enum token_type token_type;

struct token {
    char *name;
    token_type type;
};

typedef struct token token;

/* TOKEN STACK */
struct token_stack {
    token item;
    struct token_stack *next;
};

typedef struct token_stack token_stack;
typedef token_stack token_list;

#define POP_TOKEN(head) POP(head, token)
#define CLEAR_TOKEN(head) CLEAR(head, token)

token get_token(char *expr, int *start_pos);


#endif
