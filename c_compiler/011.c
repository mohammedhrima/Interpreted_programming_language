#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tokenizer (enums)
typedef enum
{
    TK_RESERVED, // keyword or punctuators
    TK_NUM,      // Integer literals
    TK_EOF       // end of file marker
} TokenKind;

typedef struct Token
{
    TokenKind kind;     // token kind
    struct Token *next; // Next token
    int val;            // if kind is TK_NUM , its value
    char *str;          // token string
} Token;

// Input program
char *user_input;

// Current token
Token *token;

// Report error and exit
void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// Report an error with its location and exit
void error_at(char *loc, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// Cosumes the current token if it matches 'op'
bool consume(char op)
{
    if (token->kind != TK_RESERVED || token->str[0] != op)
        return false;
    token = token->next;
    return true;
}

// Ensure that current token is 'op'
void expect(char op)
{
    if (token->kind != TK_RESERVED || token->str[0] != op)
        error_at(token->str, "expected %c\n", op);
    token = token->next;
}

// Ensure that the current token is TK_NUM
int expect_number()
{
    if (token->kind != TK_NUM)
        error_at(token->str, "expected a number\n");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof()
{
    return token->kind == TK_EOF;
}

// create a new token and add it as next token of "cur"
Token *new_token(TokenKind kind, Token *curr, char *str)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    curr->next = tok;
    return tok;
}

// tokinze "user_input" and returns new tokens
Token *tokenize()
{
    char *p = user_input;
    Token head;
    head.next = NULL;
    Token *curr = &head;

    int i = 0;
    while (p && p[i])
    {
        if (isspace(p[i]))
        {
            i++;
            continue;
        }
        else if (p[i] == '+' || p[i] == '-')
            curr = new_token(TK_RESERVED, curr, p + i);
        else if (isdigit(p[i]))
        {
            curr = new_token(TK_NUM, curr, p + i);
            curr->val = atoi(p + i);
        }
        else
            error_at(p + i, "expected a number\n");
        i++;
    }
    new_token(TK_EOF, curr, p + i);
    return head.next;
}

// Prasing
typedef enum
{
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_NUM  // integer
} NodeKind;

// structs
typedef struct Node
{
    NodeKind kind;
    struct Node *lhs; // left hand
    struct Node *rhs; // right hand
    int val;   // value
} Node;

// init functions
Node *new_node(NodeKind kind)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    return node;
}

Node *new_binary(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = new_node(kind);
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_num(int val)
{
    Node *node = new_node(ND_NUM);
    node->val = val;
    return node;
}

Node *expr(void);
Node *mul(void);
Node *primary(void);

// expr = mul ("+" mul | "-" mul)*
Node *expr()
{
    Node *node = mul();
    while (1)
    {
        if (consume('+'))
            node = new_binary(ND_ADD, node, mul());
        else if (consume('-'))
            node = new_binary(ND_SUB, node, mul());
        else
            return node;
    }
}

// mul = primary ("*" primary | "/" primary)*
Node *mul()
{
    Node *node = primary();

    while (1)
    {
        if (consume('*'))
            node = new_binary(ND_MUL, node, primary());
        else if (consume('/'))
            node = new_binary(ND_DIV, node, primary());
        else
            return node;
    }
}

// primary = "(" expr ")" | num
Node *primary()
{
    if (consume('('))
    {
        Node *node = expr();
        expect(')');
        return node;
    }
    return new_num(expect_number());
}

// assembly generator
void gen(Node *node)
{
    if (node->kind == ND_NUM)
    {
        printf("    push %d\n", node->val);
        return;
    }
    gen(node->lhs);
    gen(node->rhs);

    printf("    pop rdi\n");
    printf("    pop rax\n");

    switch (node->kind)
    {
    case ND_ADD:
        printf("    add rax, rdi\n");
        break;
    case ND_SUB:
        printf("    sub rax, rdi\n");
        break;
    case ND_MUL:
        printf("    imul rax, rdi\n");
        break;
    case ND_DIV:
        printf("    cqo\n");
        printf("    idiv rdi\n");
        break;
    }
    printf("    push rax\n");
}

int main(int argc, char **argv)
{
    if (argc != 2)
        error("%s: invalid number of arguments", argv[0]);

    // Tokenize and prse
    user_input = argv[1];
    token = tokenize();
    Node *node = expr();

    // head of assembly file
    printf(".intel_syntax noprefix\n");
    printf(".globl _main\n");
    printf("_main:\n");

    // build the assembly body
    gen(node);

    // A result must be at the top of the stack, so pop it
    // to RAX to make it a program exit code.
    printf("    pop rax\n");
    printf("    ret\n");
    return (0);
}