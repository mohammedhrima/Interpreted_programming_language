#include "lex.h"
#include <stdio.h>
#include <ctype.h>

char *yytext = ""; // Lexeme not '\0'
int yyleng = 0;    // Lexeme length
int yylineno = 0;  // input line number

int lex(void)
{
    static char input_buffer[128];
    char *current;

    current = yytext + yyleng;

    while (1)
    {
        while (!*current)
        {
            // Get new lines, skiping any white spaces on the line
            // until a nonblank line is found
            //

            current = input_buffer;
            if (!gets(input_buffer))
            {
                *current = '\0';
                return EOI;
            }

            ++yylineno;

            while (isspace(*current))
                ++current;
        }

        for (; *current; ++current)
        {
            // get the next token
            yytext = current;
            yyleng = 1;
            switch (*current)
            {
            case EOF:
                return EOI;
            case ';':
                return SEMI;
            case '*':
                return TIMES;
            case '+':
                return PLUS;
            case '(':
                return LP;
            case ')':
                return RP;
            case '\n':
            case '\t':
            case ' ':
                break;
            default:
                if (!isalnum(*current))
                    fprintf(stderr, "Ignoring illegal input <%c>\n", *current);
                else
                {
                    while (isalnum(*current))
                        ++current;
                    yyleng = current - yytext;
                    return NUM_OR_ID;
                }
                break;
            }
        }
    }
}

static int Lookahead = -1; // Lookhead token

int match(int token)
{
    // Return true if "token matches the current lookahead symbol."
    if (Lookahead == -1)
        Lookahead = lex();
    return token == Lookahead;
}

void advance()
{
    // Advance the lookahead to the next input symbol
    Lookahead = lex();
}
// plain
// Basic parser, shows the struture but there's no code generation

void factor()
{
    // factor -> NUM_OR_ID | LP expression RP
    if (match(NUM_OR_ID))
        advance();
    else if (match(LP))
    {
        advance();
        expression();
        if (match(RP))
            advance();
        else
            fprintf(stderr, "%d: Mismtched parenthesis\n", yylineno);
    }
    else
        fprintf(stderr, "%d Number or identifier expected\n", yylineno);
}

void term_prime()
{
    // term' -> TIMES factor term' | eps
    if (match(TIMES))
    {
        advance();
        factor();
        term_prime();
    }
}

void term()
{
    // term -> factor term'
    factor();
    term_prime();
}

void expr_prime()
{
    // expression' -> PLUS term expression' | eps
    if (match(PLUS))
    {
        advance();
        term();
        expr_prime();
    }
}

void expression()
{
    // expression -> term expression'
    term();
    expr_prime();
}

void statements()
{
    // statements -> expression SEMI | expression SEMI statements
    expression();
    if (match(SEMI))
        advance();
    else
        fprintf(stderr, "%d: Inserting missing semicolon\n", yylineno);
    if (!match(EOI))
        statements(); // do another statement
}

