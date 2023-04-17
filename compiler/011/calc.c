#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    /*
    n : number
    operator: with there symbols +, *, -, /
    */
    int type;
    char *value;
} Token;

char *text;
int txt_pos;
int tk_pos;
Token *tokens[100];

void skip_space()
{
    while (isspace(text[txt_pos]))
        txt_pos++;
}

void new_token(int start, int end, int type)
{
    tokens[tk_pos] = calloc(1, sizeof(Token));
    tokens[tk_pos]->value = calloc(1, end - start + 1);
    tokens[tk_pos]->type = type;
    memcpy(tokens[tk_pos]->value, text + start * sizeof(char), end - start);
    printf("new token with value: %s, and type: '%c'\n", tokens[tk_pos]->value, tokens[tk_pos]->type);
    tk_pos++;
    tokens[tk_pos] = NULL;
}

void build_tokens(void)
{
    int start;
    while (text[txt_pos])
    {
        skip_space();
        if (isdigit(text[txt_pos]))
        {
            start = txt_pos;
            while (isdigit(text[txt_pos]))
                txt_pos++;
            new_token(start, txt_pos, 'n');
        }
        else if (text[txt_pos] && strchr("+-/*()", text[txt_pos]))
        {
            new_token(0, 0, text[txt_pos]);
            txt_pos++;
        }
        else
        {
            printf("Unkown token '%c' in build tokens\n", text[txt_pos]);
        }
    }
    new_token(0, 0, '\0');
}

typedef struct Tree Tree;
struct Tree
{
    Tree *left;
    Tree *right;
    int type;
    char *value;
};

Tree *add();
Tree *mull();
Tree *prime();

Tree *expr()
{
    return add();
}

Tree *add()
{
    Tree *left = mull();
    while (tokens[tk_pos]->type == '+' || tokens[tk_pos]->type == '-')
    {
        Tree *node = calloc(1, sizeof(Tree));
        node->type = tokens[tk_pos]->type;
        tk_pos++;
        node->left = left;
        node->right = mull();
        left = node;
    }
    return left;
}

Tree *mull()
{
    Tree *left = prime();
    while (tokens[tk_pos]->type == '*' || tokens[tk_pos]->type == '/')
    {
        Tree *node = calloc(1, sizeof(Tree));
        node->type = tokens[tk_pos]->type;
        tk_pos++;
        node->left = left;
        node->right = prime();
        left = node;
    }
    return left;
}

Tree *prime()
{
    Tree *left = NULL;
    if (tokens[tk_pos]->type == 'n')
    {
        left = calloc(1, sizeof(Tree));
        left->type = 'n';
        left->value = tokens[tk_pos]->value;
        tk_pos++;
    }
    else if (tokens[tk_pos]->type == '(')
    {
        tk_pos++;
        // left = calloc(1, sizeof(Tree));
        left = expr();
        tk_pos++;
    }
    return left;
}

int eval(Tree *node)
{
    if (node->type == 'n')
    {
        int res = 0;
        int i = 0;
        char *value = node->value;
        while (value[i])
        {
            res = 10 * res + value[i] - '0';
            i++;
        }
        return res;
    }
    if (node->type == '+')
        return (eval(node->left) + eval(node->right));
    if (node->type == '-')
        return (eval(node->left) - eval(node->right));
    if (node->type == '*')
        return (eval(node->left) * eval(node->right));
    if (node->type == '/')
        return (eval(node->left) / eval(node->right));
    return (0);
}

int main(void)
{
#if 0
    int expected = 5 + 15 / 2 * 9 + 2 * 9 / 8 - 9 * 7;
    printf("%d\n", expected);
#endif
    FILE *fp = NULL;
    long file_size = 0;

    // open file
    fp = fopen("file.hr", "r");
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);

    text = calloc(file_size + 1, sizeof(char));
    fseek(fp, 0, SEEK_SET);
    fread(text, file_size, 1, fp);
    fclose(fp);
    printf("'%s'\n\n", text);
    build_tokens();
    int i = 0;
    while (i < tk_pos)
    {
        switch (tokens[i]->type)
        {
        case 'n':
            printf("%s\n", tokens[i]->value);
            break;
        case '+':
        case '-':
        case '*':
        case '/':
            printf("%c\n", tokens[i]->type);
            break;
        case '\0':
            printf("EOF\n");
            break;
        default:
            printf("Unknown token '%c' in printing\n", tokens[i]->type);
        }
        i++;
    }
    tk_pos = 0;
    int res = eval(expr());
    printf("res: %d\n", res);
}