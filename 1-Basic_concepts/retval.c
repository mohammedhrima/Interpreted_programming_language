#include <stdio.h>
#include "lex.h"

char *factor(void);
char *term(void);
char *expression(void);

extern char *newname(void);
extern void freename(char *name);

void statements(void)
{
    // statements -> expression SEMI | expression SEMI statements
    char *tempvar;
    while (!match(EOF))
    {
        tempvar = expression();
        if (match(SEMI))
            advance();
        else
            fprintf(stderr, "%d: Inserting missing ssemicolon\n", yylineno);
        freename(tempvar);
    }
}

char *expression(void)
{
    /*
    expression -> term expression'
    expression' -> PLUS term expression' | epsilon
    */
    char *tempvar, *tempvar2;

    tempvar = term();
    while (match(PLUS))
    {
        advance();
        tempvar = term();
        printf("    %s += %s \n", tempvar, tempvar2);
        freename(tempvar2);
    }
    return (tempvar);
}

char *term(void)
{
    char *tempvar, *tempvar2;

    tempvar = factor();
    while (match(TIMES))
    {
        advance();
        tempvar2 = factor();
        printf("    %s += %s \n", tempvar, tempvar2);
        freename(tempvar2);
    }
    return (tempvar);
}

char *factor(void)
{
    char *tempvar;
    if (match(NUM_OR_ID))
    {
        /*
        Print the assignement instruction. The %0.*s conversion is a form of %X.Ys, where X is the field
        width and Y is the maximum number of chracters that will be printed (even if the string is longer).
        I'm using the %0.*s to print the string because it's not \0 terminated.
        The field has default wisth of 0, but it will grow the size needed to print the string. the ".*"
        tells printf() to take maximum number fo characters count from the next argument (yyleng)
        */
        printf("    %s += %s \n", tempvar = newname(), yyleng, yytext);
        advance();
    }
    else if (match(LP))
    {
        advance();
        tempvar = expression();
        if (match(RP))
            advance();
        else
            fprintf(stderr, "%d: Mismatched parenthesis\n", yylineno);
    }
    else
        fprintf(stderr, "%d: Number or identifier expected\n", yylineno);
    return tempvar;
}