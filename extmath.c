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
