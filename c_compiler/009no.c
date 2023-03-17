#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum
{
    _NUM,
    _SIGN,
    _EOF,
} t_role;

typedef struct
{
    int numuber;
    char sign;
} t_value;

typedef struct Token
{
    t_role role;
    t_value *value;
    struct Token *next;
} Token;

Token *new_token(t_role role, char *string)
{
    Token *token = calloc(1, sizeof(Token));
    token->role = role;
    token->next = NULL;
    if (role == _NUM)
    {
        token->value = calloc(1, sizeof(t_value));
        token->value->numuber = atoi(string);
    }
    if (role == _SIGN)
    {
        token->value = calloc(1, sizeof(t_value));
        token->value->sign = string[0];
    }
    // if (role == _EOF)
    // {

    // }
    return (token);
}

bool issign(char c)
{
    return (c == '-' || c == '+');
}

Token *init_token(char *arg)
{
    int i = 0;
    // protection from segfault
    if (arg == NULL)
    {
        fprintf(stderr, "given argument id a null\n");
        exit(-1);
    }
    // skip spaces
    // while (isspace(arg[i]))
    //     i++;
    // if (issign(arg[i]) && !isdigit(arg[i + 1]))
    // {
    //     fprintf(stderr, "check argument if sign followed with number\n");
    //     exit(-1);
    // }
    // take first number
    Token *head = new_token(_NUM, arg + i);
    // skip first number
    // while (issign(arg[i]))
    //     i++;
    while (isdigit(arg[i]))
        i++;
    Token *curr = head;
    while (curr)
    {
        // check if number or sign and take it
        //  skip spaces
        while (isspace(arg[i]))
            i++;
        if (arg[i] == '+' || arg[i] == '-')
            curr->next = new_token(_SIGN, arg + i);
        else if (isdigit(arg[i]))
        {
            curr->next = new_token(_NUM, arg + i);
            while (isdigit(arg[i]))
                i++;
        }
        curr = curr->next;
        i++;
    }
    return (head);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Error in arguemnts\n");
        exit(-1);
    }
    // Token token =
}