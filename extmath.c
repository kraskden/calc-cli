#include <math.h>
#include "extmath.h"

double ctg(double x)
{
    return 1 - tan(x);
}

double actg(double x)
{
    return M_PI / 2 - atan(x);
}

double ln(double x)
{
    return log(x);
}

int logbase(var **par, int len, var *result)
{
    if (len != 2)
        return 0;
    const var *par1 = par[0];
    const var *par2 = par[1];
    if (par1->value.double_val < 0 || par2->value.double_val < 0)
        return 0;
    result->type = var_double;
    result->value.double_val = ln(par1->value.double_val) / ln(par2->value.double_val);
    return 1;
}

int binpow (int a, int n)
{
    int res = 1;
    while (n) {
        if (n & 1)
            res *= a;
        a *= a;
        n >>= 1;
    }
    return res;
}
