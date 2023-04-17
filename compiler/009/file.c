#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

// tokens
char *text;
int txt_pos;

typedef struct
{
    char type;
    char *value;
} Token;
/*
    Tokens type:
        v: variable
        n: number
        s: string
        + : addition
        - : minus
        * : multiplication
        / : division
*/

Token tokens_list[100];
int tk_pos;

Token *new_token(char type, int start, int end)
{
    Token *new = &tokens_list[tk_pos];
    new->type = type;
    new->value = calloc(end - start + 1, sizeof(char));
    memcpy(new->value, text + start * sizeof(char), end - start);
    printf("new token with value: \"%s\", type: \"%c\"\n", new->value, new->type);
    tk_pos++;
    return (new);
}

void skip_space()
{
    while (isspace(text[txt_pos]))
        txt_pos++;
}

void build_tokens()
{
    int start = 0;
    Token *tok;
    while (text[txt_pos])
    {
        skip_space();
        // variable
        if (isalpha(text[txt_pos]))
        {
            start = txt_pos;
            while (isalpha(text[txt_pos]) || isdigit(text[txt_pos]))
                txt_pos++;
            new_token('v', start, txt_pos);
        }
        // number
        else if (isdigit(text[txt_pos]))
        {
            start = txt_pos;
            while (isdigit(text[txt_pos]))
                txt_pos++;
            new_token('n', start, txt_pos);
        }
        // string
        else if (text[txt_pos] == '"')
        {
            start = txt_pos++;
            while (text[txt_pos] && text[txt_pos] != '"')
                txt_pos++;
            if (text[txt_pos] != '"')
            {
                printf("expected '\"'\n");
                exit(1);
            }
            new_token('s', start, txt_pos - 1);
        }
        // get operator
        else if (text[txt_pos] && strchr("+=*/", text[txt_pos]))
        {
            start = txt_pos++;
            new_token(text[start], start, txt_pos);
        }
        else
        {
            printf("Unknown token '%c' in tokenize\n", text[txt_pos]);
            exit(1);
        }
    }
    new_token('\0', txt_pos, txt_pos);
}

// typedef enum
// {
//     binary_,
//     number_,
// } Type;

typedef struct Node Node;
struct Node
{
    Node *left;
    Node *right;
    int type;
    int value;
};

Node *new_node(int type)
{
    Node *new = calloc(1, sizeof(Node));
    new->type = type;
    // new->value = number;
    return new;
}

// int tok_pos;
// Nodes
Node *add();
Node *mul();
Node *primary();

Node *expr()
{
    Node *left = add();
    return left;
}

Node *add()
{
    printf("enter add\n");
    printf("add call mull\n");
    Node *left = mul();
    while (tokens_list[tk_pos].type == '+')
    {
        tk_pos++;
        Node *node = new_node('+');
        node->left = left;
        printf("add call mull again\n");
        node->right = mul();
        left = node;
    }
    return left;
}

Node *mul()
{
    printf("enter mull\n");
    printf("mull call primary\n");
    Node *left = primary();
    while (tokens_list[tk_pos].type == '*')
    {
        tk_pos++;
        Node *node = new_node('*');
        node->left = left;
        printf("mull call primary again\n");
        node->right = primary();
        left = node;
    }
    return (left);
}

Node *primary()
{
    printf("enter primary\n");
    Node *left = NULL;
    if (tokens_list[tk_pos].type == 'n')
    {
        int res = 0;
        int i = 0;
        char *value = tokens_list[tk_pos].value;
        printf("primary found number from '%s'\n", value);
        while (value && value[i])
        {
            res = res * 10 + (value[i] - '0');
            i++;
        }
        printf("res: %d\n", res);
        left = new_node('n');
        left->value = res;
        tk_pos++;
        // return res;
    }
    // if (tokens_list[tk_pos].type == '+')
    // {
    //     tk_pos++;
    //     Node *node = new_node('+');
    //     node->left = left;
    //     node->right = mul();
    //     left = node;
    // }
    return left;
}

int eval(Node *node)
{
    if (node->type == 'n')
    {
        printf("%d\n", node->value);
        return node->value;
    }
    else if (node->type == '*')
    {
        printf("*\n");
        int left = eval(node->left);
        int right = eval(node->right);
        printf("left: %d\n", left);
        printf("right: %d\n", right);
        return (left * right);
    }
    else if (node->type == '+')
    {
        printf("+\n");
        int left = eval(node->left);
        int right = eval(node->right);
        printf("left: %d\n", left);
        printf("right: %d\n", right);
        return (left + right);
    }
    return -1;
}

int main(void)
{
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
    printf("\n");
    int i = 0;
    while (i < tk_pos)
    {
        switch (tokens_list[i].type)
        {
        case 'n':
            printf("number: '%s'\n", tokens_list[i].value);
            break;
        case 'v':
            printf("variable: '%s'\n", tokens_list[i].value);
            break;
        case 's':
            printf("string: '%s'\n", tokens_list[i].value);
            break;
        case '+':
            printf("plus: '%s'\n", tokens_list[i].value);
            break;
        case '-':
            printf("minus: '%s'\n", tokens_list[i].value);
            break;
        case '/':
            printf("divition: '%s'\n", tokens_list[i].value);
            break;
        case '*':
            printf("multiply: '%s'\n", tokens_list[i].value);
            break;
        case '=':
            printf("assign: '%s'\n", tokens_list[i].value);
            break;
        case '\0':
            printf("EOF: '%s'\n", tokens_list[i].value);
            break;
        default:
            printf("unknown token '%c' in printing\n", tokens_list[i].type);
        }
        i++;
    }
    printf("\n");
    tk_pos = 0;

    int final_res = eval(expr());
    printf("\n\nfinal_res: %d\n ", final_res);
}