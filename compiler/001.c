#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct 
{
    char type;
    int number;
} Value;


typedef struct Node {
    struct Node*left;
    struct Node*right;
    Value *value;
} Node;

char *str = "1+2";
/*
number   +   number
*/
int i;

Node *new_node(Value *value)
{
    Node *new = calloc(1,sizeof(Node));
    new->value = value;
    return new;
}

Value *new_value(char c)
{
    Value *new = calloc(1,sizeof(Value));
    if(c == 'n')
    {
        int res = 0;
        while(isdigit(str[i]))
        {
            res = res *10 + str[i] - '0';
            i++;
        }
        printf("found number %d\n", res);
        new->type = c;
        new->number = res;
    }
    else
    {
        printf("found +\n");
        new->type = c;
        i++;
    }
    return new;
}

// expr
// + -
// number
Node *expr();
Node *add_sub();
Node *prime();

Node *expr()
{
    return add_sub();
}

Node *add_sub()
{
    Node *left = prime();
    if(str[i] == '+')
    {
        Value *value = new_value('+');
        Node *node = new_node(value);
        node->left = left;
        node->right = prime();
        return node;
    }
    return left;
}

Node *mul_div()
{
    
}
Node *prime()
{
    Node *node = NULL;
    if(isdigit(str[i]))
    {
        Value *value = new_value('n');
        node = new_node(value);
    }
    return node;
}

int eval(Node *node)
{
    printf("evaluate type %c\n", node->value->type);
    if(node->value->type == 'n')
    {
        return node->value->number;
    }
    else if(node->value->type == '+')
    {
        int left = eval(node->left);
        int right = eval(node->right);
        return left + right; 
    }
    return 0;
}

void execute()
{
    Node *curr = expr();
    int res = 0;
    while(curr)
    {
        res = eval(curr);
        curr = expr();
    }
    printf("-> %d\n", res);
}

int main(void)
{
    execute();
}