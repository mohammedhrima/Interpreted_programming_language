#include "header.h"

typedef enum
{
    INTEGER_TOKEN,
    EOF_TOKEN,
} Token_type;

typedef struct
{
    Token_type type;
    union
    {
        int int_val;
        float float_val;
        char *str_val;
    };
} Token;

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    int type;
    Token token;
} Node;

enum
{
    BINARY_NODE,
    INTEGER_NODE,
};

Node *term(void);
Node *add_sub(void);
Node *mul_div(void);
Node *expr(void);
Node *parse(Token *tok);

Token *tokens;
int count = 0;

Token *new_token(char *input)
{
    Token *new = calloc(ft_strlen(input) + 1, sizeof(Token));
    int i = 0;
    int j = 0;
    while (input && input[i])
    {
        Token *curr = &new[j];
        // printf("get token %d\n", i);
        while (isspace(input[i]))
            i++;
        if (!input[i])
            break;
        if (isdigit(input[i]))
        {
            // if it's number type will be INTEGER, then skip all numbers
            curr->type = INTEGER_TOKEN;
            while (isdigit(input[i]))
            {
                curr->int_val = curr->int_val * 10 + (input[i] - '0');
                i++;
            }
        }
        else
        {
            curr->type = input[i];
            i++;
        }
        j++;
    }
    new[j].type = '\0';
    return (new);
}

Node *new_node(int type)
{
    Node *node = calloc(1, sizeof(Node));
    node->type = type;
    node->token = tokens[count];
    return (node);
}

Node *term(void)
{
    Node *node = 0;
    if (tokens[count].type == '(')
    {
        count++;
        node = expr();
        count++;
    }
    else if (tokens[count].type == INTEGER_TOKEN)
    {
        node = new_node(INTEGER_NODE);
        count++;
    }
    return (node);
}

Node *mul_div(void)
{
    Node *node = term();
    while (1)
    {
        if (tokens[count].type == '*' || tokens[count].type == '/')
        {
            Node *new = new_node(BINARY_NODE);
            node->left = node;
            count++;
            new->right = term();
            node = new;
        }
        else
            break;
    }
    return node;
}

Node *add_sub(void)
{
    Node *node = mul_div();
    while (1)
    {
        if (tokens[count].type == '+' || tokens[count].type == '-')
        {
            Node *new = new_node(BINARY_NODE);
            new->left = node;
            count++;
            new->right = mul_div();
            node = new;
        }
        else
            break;
    }
    return node;
}

Node *expr(void)
{
    return add_sub();
}

Node *parse(Token *tok)
{
    tokens = tok;
    return expr();
}

int interp(Node *node)
{
    if (!node)
        return 0;
    if (node->type == INTEGER_NODE)
        return node->token.int_val;
    else if (node->type == BINARY_NODE)
    {
        int left = interp(node->left);
        int right = interp(node->right);
        if (node->token.type == '*')
            return left * right;
        else if (node->token.type == '/')
            return left / right;
        else if (node->token.type == '+')
            return left + right;
        else if (node->token.type == '-')
            return left - right;
    }
    return 0;
}

int main(void)
{
    while (1)
    {
        ft_fprintf(STDOUT_FILENO, "c_calc $> ");
        char *text = readline(STDIN_FILENO);
        if (text)
        {
            if (text[0] == '\n' || text[0] == '\0')
                break;
            text[ft_strlen(text) - 1] = '\0'; // replace '\n' with '\0'
            count = 0;                        // refresh count each new line
            Token *tokens = new_token(text);

            Node *node = parse(tokens);
            int value = interp(node);
            printf("value: %d\n", value);
        }
        free(text);
    }
}
