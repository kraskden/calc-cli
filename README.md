# calc-cli
Simple CLI calculator with user-defined functions and vars

# Usage:

## Calculations:
Just type math expression and press Enter. 
For example:
```
2+sin(pi/2)+cos(pi)-e^2
```
## Defining vars:
You can define come math constants and variables.
For example:
```
my_var = 100 / cos(pi) 
```
This expression define variable my_var with value -100
And you can used this var in math expressions: 
```
2+cos(my_var - 2)
```
Variable "ans" was automatically defined and contain last result of calculations

## Defining functions:
Also is possible to define functions, such as:
```
f(x) = x * x - 2 * x
```
or
```
g(x; y) = sin(x)^2 + cos(y)^2
```
After this you can use this functions on expressions:
```
2 + my_var + f(10) / g(10; pi)
```
