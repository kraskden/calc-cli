#include "token.h"
#include "extmath.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void skip_spaces(char *expr, int *pos)
{
    int i;
    for (i = *pos; i < (int)strlen(expr); ++i)
        if (expr[i] != ' ' && expr[i] != '\t')
            break;
    *pos = i;
}

token get_token(char *expr, int *start_pos)
{
    token ret;
    ret.type = token_empty;
    if (!*expr)
        return ret;
    int pos = *start_pos;
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
    *start_pos = pos;
    free(token_name);
    return ret;
}
