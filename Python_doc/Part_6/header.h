#ifndef HEADER_H
#define HEADER_H

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdarg.h>

typedef enum
{
    INTEGER_TOK,
    PLUS_TOK,
    MINUS_TOK,
    MULTIPLY_TOK,
    DIVIDE_TOK,
    EOF_TOK,
    ERROR_TOK
} Token_type;

typedef union
{
    int integer;
    char *string;
} var;

typedef struct
{
    Token_type type;
    var *value;
} Token;

typedef struct
{
    char *text;
    int position;
} Lexer;

typedef struct
{
    Lexer *lexer;
    Token *current_token;
} Interpreter;


void ft_putchar(int fd, char c);
void ft_putstr(int fd, char *str);
void error(char *str);
int ft_strlen(char *str);
void ft_strcpy(char *dest, char *src);
char *strjoin(char *string1, char *string2);
char *readline(int fd);
void ft_fprintf(int fd, char *fmt, ...);

#endif