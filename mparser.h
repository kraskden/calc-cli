#ifndef MPARSES_H
#define MAPRSES_H

#include "var.h"

enum parses_ret {
    prs_def_var, prs_def_fun, prs_err, prs_produse
};

typedef enum parses_ret parses_ret;

void mparser_init(void);
void mparser_destroy(void);
parses_ret mparser_parse(const char *expr, var *out);
#endif
