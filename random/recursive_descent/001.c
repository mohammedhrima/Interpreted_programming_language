/*
  simple calculator implemented via recursive descent
  add_op := + | -
  mul_op := * | /
  digits := {+|-} [0..9] {[0..9]}
  expr   := term {add_op term}
  term   := factor {mul_op factor}
  factor := digits | '(' expr ')'
 */

typedef int calcint_t;
calcint_t expr(void);

char token;

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

void error(const char *msg)
{
    fputs(msg, stderr);
    exit(1);
}

// match expected and move ahead
void match(char expected)
{
    if (token == expected)
    {
        token = getchar();
        return;
    }

    fprintf(stderr, "Expected %c, got %c", expected, token);
    exit(1);
}

calcint_t factor(void)
{
    calcint_t value;

    if (token == '(')
    {
        match('(');
        value = expr();
        match(')');
    }
    else if (isdigit(token) || token == '+' || token == '-')
    {
        ungetc(token, stdin);
        scanf("%d", &value);
        token = getchar();
    }
    else
    {
        error("bad factor");
    }

    return value;
}

calcint_t term(void)
{
    calcint_t value = factor();

    while (token == '*' || token == '/')
    {
        switch (token)
        {
        case '*':
            match('*');

            value *= factor();
            break;

        case '/':
            match('/');
            value /= factor();
            break;

        default:
            error("bad term");
        }
    }

    return value;
}

calcint_t expr()
{
    calcint_t value = term();

    if (token == '+' || token == '-')
    {
        switch (token)
        {
        case '+':
            match('+');
            value += term();
            break;

        case '-':
            match('-');
            value -= term();
            break;
        default:
            error("bad expr");
        }
    }

    return value;
}

int main(void)
{

    token = getchar();
    calcint_t result = expr();
    printf("result: %d\n", result);

    return 0;
}