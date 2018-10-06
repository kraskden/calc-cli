#ifndef EXTMATH_H
#define EXTMATH_H

#define DOT '.'

#include "var.h"

double ctg(double x);
double actg(double x);
double ln(double x);
int logbase(var **par, int len, var *result);
int binpow (int a, int n);

#endif
