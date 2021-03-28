#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#pragma once

typedef double(* operator_action_t)(double left, double right);

typedef struct Operator
{
    char sign;
    unsigned short priority;
    operator_action_t action;
} Operator_s;

#define OPERATORS_COUNT 7

Operator_s *new_operator(char sign, unsigned short priority, operator_action_t action)
{
    Operator_s *result = (Operator_s *)malloc(sizeof(Operator_s));
    result->sign = sign;
    result->priority = priority;
    result->action = action;
    return result;
}

double add(double left, double right)
{ return left + right; }

double differency(double left, double right)
{ return left - right; }

double product(double left, double right) 
{ return left * right; }

double division(double left, double right)
{ return left / right; }

double modulo_division(double left, double right) 
{ return left - right  * floor(left / right);}

double whole_division(double left, double right)
{ return round(left / right); }

double exponentiation(double base, double exponent)
{ return pow(base, exponent); }

Operator_s **init_operators()
{
    Operator_s **operators = (Operator_s **)malloc(OPERATORS_COUNT * sizeof(Operator_s *));

    operators[0] = new_operator('+', 1, add);
    operators[1] = new_operator('-', 1, differency);
    operators[2] = new_operator('*', 2, product);
    operators[3] = new_operator('/', 2, division);
    operators[4] = new_operator('%', 2, modulo_division);
    operators[5] = new_operator('\\', 2, whole_division);
    operators[6] = new_operator('^', 3, exponentiation);

    return operators;
}

short check_operator(char _char, Operator_s **operators)
{
    for (size_t i = 0; i < OPERATORS_COUNT; i++)
        if (operators[i]->sign == _char) 
            return 1;

    return 0;
}

short get_priority(char _char, Operator_s **operators)
{
    for (size_t i = 0; i < OPERATORS_COUNT; i++)
        if (operators[i]->sign == _char) return operators[i]->priority;

    return -1;
}