#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdarg.h>

typedef enum
{
    INTEGER_TOK = 256,
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

enum
{
    BINARY_NODE,
    INTEGER_NODE,
};

typedef struct Node Node;
struct Node
{
    Node *left;
    Node *right;
    int type;
    Token token;
};

void ft_putchar(int fd, char c);
void ft_putstr(int fd, char *str);
void error(char *str);
int ft_strlen(char *str);
void ft_strcpy(char *dest, char *src);
char *ft_strdup(char *str);
char *strjoin(char *string1, char *string2);
char *readline(int fd);
void ft_fprintf(int fd, char *fmt, ...);
// PARSING FUNCTIONS

#endif