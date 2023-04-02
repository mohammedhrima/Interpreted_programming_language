#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

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
        error("Error in expect: %c\n", op);
    token = token->next;
}

int expect_number()
{
    if (token->kind != TK_NUM)
        error("Error in expect_number\n");
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
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    curr->next = tok;
    return tok;
}

Token *tokenize(char *p)
{
    Token head;
    head.next = NULL;
    Token *curr = &head;
    while (*p)
    {
        if (isspace(*p))
        {
            p++;
            continue;
        }
        if (*p == '+' || *p == '-')
        {
            curr = new_token(TK_RESERVED, curr, p++);
            continue;
        }
        if (isdigit(*p))
        {
            curr = new_token(TK_NUM, curr, p);
            curr->val = strtol(p, &p, 10);
            continue;
        }
        error("error in tokensize\n");
    }
    new_token(TK_EOF, curr, p);
    return head.next;
}

char *user_input;

void error_at(char *loc, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        error("error in arguments");
        return 1;
    }
    printf("argv[1]: %s\n", argv[1]);
    // exit(0);
    token = tokenize(argv[1]);

    // int fd = open("file.asm", O_CREAT | O_TRUNC | O_RDWR);
    // dup2(fd, STDOUT_FILENO);
    printf(".intel_syntax noprefix\n");
    printf(".global _main\n\n");
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
    printf("    ret\n");
    return 0;
}