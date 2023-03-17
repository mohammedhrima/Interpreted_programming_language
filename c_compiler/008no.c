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

// Token struct
typedef struct Token
{
    TokenKind kind;
    int val;
    char c; // to save sign + / -
    struct Token *next;
} Token;

// declare my token
Token *token;

int expected_number()
{
    if (token->kind != TK_NUM)
        fprintf(stderr, "expected a number but found something else\n");
    int val = token->val;
    token = token->next;
    return val;
}
// new token
Token *new_token(TokenKind kind, Token *curr, char c)
{
    Token *new = calloc(1, sizeof(Token));
    new->kind = kind;
    new->c = c;
    curr->next = new;
    return new;
}
// build token
Token *tokenize(char *arg)
{
    Token head;
    head.next = NULL;
    Token *curr = &head;

    int i = 0;
    while (arg && arg[i])
    {
#if 1
        printf("arg[%d] = %c\n", i, arg[i]);
// #else
        while (isspace(arg[i]))
            i++;
        if (arg[i] == '+' || arg[i] == '-')
            curr = new_token(TK_RESERVED, curr, arg[i]);
        else if (isdigit(arg[i]))
        {
            curr = new_token(TK_NUM, curr, arg[i]);
            curr->val = atoi(arg + i);
            while (isdigit(arg[i]))
                i++;
        }
        else
        {
            fprintf(stderr, "argument is neither a numebr nor a sign \"%s\"\n", arg + i);
            exit(-1);
        }
#endif
        i++;
    }
    new_token(TK_EOF, curr, arg[i]);
    return head.next;
}
// check if end of file
bool at_eof()
{
    return token->kind == TK_EOF;
}
// find sign + / -
bool find(char op)
{
    if (token->kind != TK_RESERVED || token->c != op)
        return false;
    token = token->next;
    return true;
}

// void
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Error in arguments\n");
        exit(-1);
    }
    // feed token
    token = tokenize(argv[1]);

    // head of assmebly file
    printf(".intel_syntax noprefix\n");
    printf(".globl _main\n");
    printf("_main:\n");
    // put first number
    printf("    mov rax, %d\n", expected_number());
    while (!at_eof())
    {
        if (find('+'))
            printf("    add rax, %d\n", expected_number());
        else if (find('-'))
            printf("    sub rax, %d\n", expected_number());
        else
            fprintf(stderr, "error");
        token = token->next;
    }
    printf("    ret\n");
    return (0);
}