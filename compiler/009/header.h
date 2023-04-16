#ifndef HEADER_H
#define HEADER_H

#define STDIN STDIN_FILENO
#define STDOUT STDOUT_FILENO
#define STDERR STDERR_FILENO

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdarg.h>
#include <signal.h>
#include <math.h>
#include <stdbool.h>

typedef enum
{
    left_dir,
    right_dir
} dir;

typedef enum
{
    characters_,
    integer_,
    float_,
    boolean_,
    array_,
    object_,
    none_
} data;

typedef struct
{
    char *name;
    union
    {
        // long integer;
        double number;
        char *string;
        bool boolean;
    } value;
    int curr_index;
    bool temporary;
    data type;
} var;

typedef struct Token Token;
struct Token
{
    // char *left; //wil be replace with var*
    // var *variable; // will be replaced with var*
    char *value;
    Token *right;
};

// int var_index;
var *variables[500];

// int tmp_index;
var *temporaries[500];

// readen lines
int pos;
int line;
char *text;

char *readline(int fd);
void ft_printf(int fd, char *fmt, ...);
int ft_strlen(char *str);
void handle_signal(int signum);
void ft_strcpy(char *dest, char *src);
void ft_strncpy(char *dest, char *src, int size);
void print_space(int fd, int line_long, char c);
int ft_isalpha(int c);
int ft_isdigit(int c);
int ft_isspace(int c);
char *ft_strchr(char *s, int c);
char *ft_strrchr(char *s, int c);
int ft_strncmp(char *s1, char *s2, size_t n);
int ft_strcmp(char *s1, char *s2);
double ft_atof(char *str);
void ft_putfloat(int fd, double num, int decimal_places);
char *ft_strnstr(char *haystack, char *needle, size_t len);
char *get_identifier(int start, int end);
var *get_variable_from_stock(char *name);
var *less_than_more_than(var *left, var *right, int operation);
var *new_var(char *name, data type, bool temporary);
var *operation_(var *left, var *right, int operation);
var *operation__(var *left, var *right, char *operation);
char *to_string(data type);
void *ft_memcpy(void *dst, void *src, size_t n);
void assign_var(var *dest, var *src);
void visualize_variables(void);
int is_math_operation(int c);
int skip_space(void);
Token *assign();
void indexes();
void ft_putstr(int fd, char *str);
void ft_putnbr(int fd, long num);
void skip_alpha_num(void);
Token *new_token(char *value);
#endif