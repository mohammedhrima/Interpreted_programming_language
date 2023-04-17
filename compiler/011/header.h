#ifndef HEADER_H
#define HEADER_H

#define in STDIN_FILENO
#define out STDOUT_FILENO
#define err STDERR_FILENO

#include <stdio.h>
// #include <ctype.h>
// #include <string.h>
#include <stdlib.h>
// #include <sys/types.h>
// #include <sys/uio.h>
#include <unistd.h>
#include <stdarg.h>
// #include <ctype.h>
// #include <signal.h>
#include <math.h>
// #include <stdbool.h>

// enums
typedef enum
{
    eof_,
    variable_,
    characters_,
    number_,
    assign_,
    equal_,
    less_than_,
    more_than_,
    function_,
    if_,
    while_,
    and_,
    or_,
    lparent_,
    rparent_
} tok_type;

// structs
typedef struct
{
    char *value;
    tok_type type;
} Token;

Token multi_tokens[] = {
    {"if", if_},
    {"while", while_},
    {"or", or_},
    {"and", and_},
    {"func", function_},
    {"while", while_},
    {"==", equal_},
    {"<=", less_than_},
    {">=", more_than_},
    {"=", assign_},
    {"(", lparent_},
    {")", rparent_},
    {0, 0},
};

// tokens
Token *tokens[1000];
extern int tk_pos;

// readen lines
int txt_pos;
int line;
char *text;

char *readline(int fd);
void ft_putnbr(int fd, long num);
void ft_putstr(int fd, char *str);
void ft_printf(int fd, char *fmt, ...);
int ft_strlen(char *str);
void handle_signal(int signum);
void ft_strcpy(char *dest, char *src);
void ft_strncpy(char *dest, char *src, int size);
void print_space(int fd, int line_long);
int ft_isalpha(int c);
int ft_isdigit(int c);
int ft_isspace(int c);
int ft_isalnum(int c);
char *ft_strchr(char *s, int c);
char *ft_strrchr(char *s, int c);
int ft_strncmp(char *s1, char *s2, size_t n);
int ft_strcmp(char *s1, char *s2);
double ft_atof(char *str);
void ft_putfloat(int fd, double num, int decimal_places);
char *ft_strnstr(char *haystack, char *needle, size_t len);
void indexes();

#endif