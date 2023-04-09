#include "header.h"

Token *tokenize(char *s)
{
    Token *tokens = calloc(sizeof(Token), (ft_strlen(s) + 1));
    int i = 0, j = 0;
    while (s[i])
    {
        Token *token = &tokens[j];
        while (isspace(s[i]))
            i++;
        if (!s[i])
            break;
        if (isdigit(s[i]))
        {
            token->type = INTEGER_TOKEN;
            while (isdigit(s[i]))
            {
                token->int_val = token->int_val * 10 + (s[i] - '0');
                i++;
            }
        }
        else
        {
            token->type = s[i];
            i++;
        }
        j++;
    }
    tokens[j].type = 0;
    return tokens;
}

Token *tokens;
int c = 0;

Node *new_node(int type)
{
    Node *node = malloc(sizeof(Node));
    node->type = type;
    node->token = tokens[c];
    return node;
}

void skip(int type)
{
    if (tokens[c].type == type)
        c++;
}

Node *term();
Node *add();
Node *mul();

Node *expr()
{
    return add();
}

Node *add()
{
    Node *node = mul();
    while (1)
    {
        if (tokens[c].type == '+' || tokens[c].type == '-')
        {
            Node *new = new_node(BINARY_NODE);
            new->left = node;
            c++;
            new->right = mul();

            node = new;
        }
        else
            break;
    }
    return node;
}

Node *mul()
{
    Node *node = term();
    while (1)
    {
        if (tokens[c].type == '*' || tokens[c].type == '/')
        {
            Node *new = new_node(BINARY_NODE);
            new->left = node;
            c++;
            new->right = term();
            node = new;
        }
        else
            break;
    }
    return node;
}

Node *term()
{
    Node *node = 0;
    if (tokens[c].type == '(')
    {
        c++;
        node = expr();
        c++;
    }
    else if (tokens[c].type == INTEGER_TOKEN)
    {
        node = new_node(INTEGER_NODE);
        c++;
    }
    return node;
}

Node *parse(Token *_tokens)
{
    tokens = _tokens;
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
        ft_fprintf(stdout, "c_calc $> ");
        char *text = readline(stdin);
        if (text)
        {
            if (text[0] == '\0' || text[0] == '\n')
                break;
            text[ft_strlen(text) - 1] = '\0'; // replace '\n' with '\0'

            c = 0;
            Token *tokens = tokenize(text);
            Node *node = parse(tokens);
            int val = interp(node);
            printf("value: %d\n", val);
        }
        free(text);
    }
}