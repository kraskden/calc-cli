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

#define VAR_LENGTH 255

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

#define FUNC_NAME_SIZE 255
#define EXPR_SIZE 255

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
