#include <stdio.h>
#include "lex.h"

void factor(char *tempvar);
void term(char *tempvar);
void expression(char *tempvar);
extern char *newname(void);
extern void freename(char *name);

void statement(void)
{
    // statement -> expression SEMI | expression SEMI statements
    char *tempvar;

    while (!match(EOI))
    {
        expression(tempvar = newname());
        freename(tempvar);
        if (match(SEMI))
            advanced();
        else
            fprintf(stderr, "%d: Inserting missing semicolon\n", yylineno);
    }
}

void expression(char *tempvar)
{
    // expression -> term expression'
    // expression' -> PLUS term expression' | epsilon
    char *tempvar2;

    term(tempvar);
    while (match(PLUS))
    {
        advanced();
        term(tempvar2 = newname());
        printf("    %s += %s \n", tempvar, tempvar2);
        freename(tempvar2);
    }
}

void term(char *tempvar)
{
    char *tempvar2;

    factor(tempvar);
    while (match(TIMES))
    {
        advanced();
        factor(tempvar2 = newname());
        printf("    %s *= %s \n", tempvar, tempvar2);
        freename(tempvar2);
    }
}

void factor(char *tempvar)
{
    if (match(NUM_OR_ID))
    {
        printf("    %s += %s \n", tempvar, yyleng, yytext);
        advanced();
    }
    else if (match(LP))
    {
        advanced();
        expression(tempvar);
        if (match(RP))
            advanced();
        else
            fprintf(stderr, "%d: Mismatched parenthesis\n", yylineno);
    }
    else
        fprintf(stderr, "%d:Number or identifier expected\n", yylineno);
}