#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// enums
typedef enum
{
    TK_RESERVED,
    TK_NUM,
    TK_EOF
} TokenKind;

typedef struct Token
{
    TokenKind kind;
    struct Token *next;
    int val;
    char *str;
} Token;

Token *token;

void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

bool consume(char op)
{
    if (token->kind != TK_RESERVED || token->str[0] != op)
        return false;
    token = token->next;
    return true;
}

void expect(char op)
{
    if (token->kind != TK_RESERVED || token->str[0] != op)
        error("Error 1 bi ljapniya: %c\n", op);
    token = token->next;
}

int expect_number()
{
    if (token->kind != TK_NUM)
        error("error 2 bi ljaponiya\n");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof()
{
    return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *curr, char *str)
{
    Token *token1 = calloc(1, sizeof(Token));
    token1->kind = kind;
    token1->str = str; // to be removerd after o guess
    curr->next = token1;
    return token1;
}

Token *tokenize(char *p)
{
    Token head;
    head.next = NULL;
    Token *curr = &head;

    int i = 0;
    while (p && p[i])
    {
        if (isspace(p[i]))
            i++;
        else if (p[i] == '+' || p[i] == '-')
            curr = new_token(TK_RESERVED, curr, p + i);
        else if (isdigit(p[i]))
        {
            curr = new_token(TK_NUM, curr, p + i);
            curr->val = atoi(p + i);
        }
        else
            error("error 3 bi ljaponiya\n");
        i++;
    }
    new_token(TK_EOF, curr, p + i);
    return head.next;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        error("error 4 bi ljaponiya\n");
        return 1;
    }
    token = tokenize(argv[1]);

    // head of assembly file
    printf(".intel_syntax noprefix\n");
    printf(".globl _main\n");
    printf("_main:\n");

    printf("    mov rax, %d\n", expect_number());

    while (!at_eof())
    {
        if (consume('+'))
        {
            printf("    add rax, %d\n", expect_number());
            continue;
        }
        expect('-');
        printf("    sub rax, %d\n", expect_number());
    }
    printf("  ret\n");
    return (0);
}