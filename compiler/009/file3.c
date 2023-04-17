#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// tokens
typedef enum
{
    eof_,
    number_,
    var_,
    string_,
} Type_tk;

typedef struct
{
    char *name;
    union
    {
        char *character;
        int number;
    } value;
} data;

typedef struct Token
{
    Type_tk type;
    data val;
} Token;

char *text;
int txt_pos;
Token tokens[1000];
int tok_pos;

void build_tokens()
{
    int start = 0;
    int end = 0;

    while (text[txt_pos])
    {
        while (isspace(text[txt_pos]))
            txt_pos++;
        if (text[txt_pos] == '\0')
            break;
        Token *token = &tokens[tok_pos];
        // get number
        if (isdigit(text[txt_pos]))
        {
            token->type = number_;
            while (isdigit(text[txt_pos]))
            {
                token->val.value.number = token->val.value.number * 10 + (text[txt_pos] - '0');
                txt_pos++;
            }
        }
        // get identifier name
        else if (isalpha(text[txt_pos]))
        {
            token->type = var_;
            start = txt_pos;
            while (isalnum(text[txt_pos]))
                txt_pos++;
            token->val.value.character = calloc(txt_pos - start + 1, sizeof(char));
            memcpy(token->val.value.character, text + start, txt_pos - start);
        }
        // get string
        else if (text[txt_pos] == '"')
        {
            token->type = string_;
            txt_pos++; // skip first "
            start = txt_pos;
            while (text[txt_pos] && text[txt_pos] != '"')
                txt_pos++;
            token->val.value.character = calloc(txt_pos - start + 1, sizeof(char));
            memcpy(token->val.value.character, text + start, txt_pos - start);
            txt_pos++; // skip second "
        }
        // get operator
        else if (text[txt_pos] && strchr("=+", text[txt_pos])) // replace it with ft_strchr
        {
            token->type = text[txt_pos]; // type will get ascci of operator
            txt_pos++;
        }
        else
        {
            printf("error: unkown token '%c'\n", text[txt_pos]);
            exit(0);
        }
        // printf("\ntxt_pos: %d", txt_pos);
        tok_pos++;
    }
    tokens[tok_pos++].type = eof_;
}

// parsing
typedef enum
{
    node_binary,
    node_number,
    node_string,
    node_assign,
    node_var
} Type_nd;

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    int nd_type;
    // Type_tk tk_type;
    data *var;
} Node;
//////////////////
Node *new_node(Type_nd nd_type)
{
    Node *new = calloc(1, sizeof(Node));
    new->left = NULL;
    new->right = NULL;
    new->nd_type = nd_type;
    // new->tk_type = tk_type;
    return (new);
}
//////////////////
// variables
data vars[100];
int var_count;
int parse_pos;

data *append_var(char *name)
{
    data *new = &vars[var_count++];
    new->value.character = name;
    return new;
}

data *find_var(char *name)
{
    int i = 0;
    while (i < var_count)
    {
        if (strcmp(name, vars[i].name) == 0)
            return &(vars[i]);
        i++;
    }
    return NULL;
}

Node *assign();
Node *add();
Node *primary();

// get last value
Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *left = add();
    if (tokens[parse_pos].type == '=')
    {
        Node *node = new_node(node_assign, '=');
        parse_pos++;
        node->left = left;
        if (node->left->nd_type != node_var)
        {
            printf("error: expected a varibale\n");
            exit(0);
        }
        node->right = assign();
        return node;
    }
    return left;
}

Node *add()
{
    Node *left = primary();
    while (tokens[parse_pos].type == '+')
    {
        Node *node = new_node(node_binary, '+');
        parse_pos++;
        // put previous number on left
        node->left = left;
        // put next number on right
        node->right = primary();
        // reset left
        left = node;
    }
    return left;
}

Node *primary()
{
    if (tokens[parse_pos].type == number_)
    {
        Node *node = new_node(node_number);
        parse_pos++;
        return node;
    }
    else if (tokens[parse_pos].type == var_)
    {
        Node *node = new_node(node_var);
        node->var = find_var(tokens[parse_pos].val.name);
        if (node->var == NULL)
            node->var = append_var(tokens[parse_pos].val.name);
        parse_pos++;
        return node;
    }
    else if (tokens[parse_pos].type == eof_)
        parse_pos++;
    // else if (tokens[parse_pos].type == string_)
    // {
    //     Node *node = new_node(node_string);
    //     parse_pos++;
    //     return node;
    // }
    else
    {
        printf("error: unknown type '%c' '%d' \n", tokens[parse_pos].type, tokens[parse_pos].type);
        exit(1);
    }
    return NULL;
}
#include <assert.h>
int eval(Node *node)
{
    if (node->nd_type == node_number)
    {
        printf("is a number '%d'\n", node->var->value.number);
        return node->var->value.number;
    }
    else if (node->nd_type == node_var)
    {
        printf("is a variable '%s'\n", node->var->name);
        return node->var->value.number;
    }
    else if (node->nd_type == node_assign)
    {
        node->left->var->value.number = eval(node->right);
        return node->left->var->value.number;
    }
    else if (node->nd_type == node_binary)
    {
        int left = eval(node->left);
        int right = eval(node->right);
        switch (node->nd_type)
        {
        case '+':
            return left + right;
        case '-':
            return left + right;
        case '/':
            return left + right;
        case '*':
            return left + right;
        default:
            assert(0);
        }
    }
    assert(0);
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
    printf("%s\n", text);
    int i = 0;
    while (text[i])
    {
        printf("%d", i % 10);
        i++;
    }
    printf("\n");
    build_tokens();
    for (int i = 0; i < tok_pos; i++)
    {
        Token token = tokens[i];
        if (token.type == eof_)
            printf("token type: eof\n");
        else if (token.type == number_)
            printf("token type: number, with value '%d'\n", token.val.value.number);
        else if (token.type == var_)
            printf("token type: var, with value '%s'\n", token.val.value.character);
        else if (token.type == string_)
            printf("token type: string, with value '%s'\n", token.val.value.character);
        else
            printf("token type: '%c'\n", token.type);
    }
    // while (tokens[parse_pos].type)
    //     printf("%d\n", eval(expr()));
    // expr();
    // printf("%d\n", i);
}