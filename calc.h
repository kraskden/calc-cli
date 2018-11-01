#ifndef CALC_H
#define CALC_H

#include "var.h"
#include "token.h"

int produse(char *expr, var *out);
token_list *polish_convert(char *expr);
//int calculate(token_list *head, var *out);

#endif
