#include "header.h"
Token *tokens;
int count = 0;

Token *new_token(char *str)
{
    Token *new = calloc(ft_strlen(str) + 1, sizeof(Token));
    int i = 0;
    int j = 0;
    while (str && str[i])
    {
        Token *curr = &new[j];
        while (isspace(str[i]))
            i++;
        if (!str[i])
            break;
        if (isdigit(str[i]))
        {
            curr->type = INTEGER_TOKEN;
            while (isdigit(str[i]))
            {
                curr->int_val = curr->int_val * 10 + (str[i] - '0');
                i++;
            }
        }
        else
        {
            curr->type = str[i];
            i++;
        }
        j++;
    }
    new[j].type = '\0';
    return new;
}

Node *new_node(int type)
{
    Node *node = calloc(1, sizeof(Node));
    node->type = type;
    node->token = tokens[count];
    return node;
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
    return node;
}

Node *mul_div(void)
{
    Node *node = term();
    while (1)
    {
        if (tokens[count].type == '*' || tokens[count].type == '/')
        {
            Node *new = new_node(BINARY_NODE);
            new->left = node;
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