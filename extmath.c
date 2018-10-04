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

int logbase(var **par, int len, double *result)
{
    if (len != 2)
        return 0;
    const var *par1 = par[0];
    const var *par2 = par[1];
    if (par1->value.double_val < 0 || par2->value.double_val < 0)
        return 0;
    *result =  ln(par1->value.double_val) / ln(par2->value.double_val);
    return 1;
}
