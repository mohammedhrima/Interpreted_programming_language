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
#include <stdbool.h>

// tokens
typedef enum
{
    eof_,
    variable_,
    characters_,
    boolean_,
    number_,
    integer_,
    float_,
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
    rparent_,
    lbracket_,
    rbracket_,
    array_,
    output_,
    input_,
    Node_,
} Type;

typedef struct
{
    char *value;
    Type type;
} Token;

Token multi_tokens1[] = {
    {"if", if_},
    {"while", while_},
    {"or", or_},
    {"and", and_},
    {"func", function_},
    {"while", while_},
    // {"==", equal_},
    // {"<=", less_than_},
    // {">=", more_than_},
    // {"=", assign_},
    // {"(", lparent_},
    // {")", rparent_},
    // {"[", lbracket_},
    // {"]", rbracket_},
    {"output", output_},
    {"input", input_},
    {0, 0},
};

Token multi_tokens2[] = {
    // {"if", if_},
    // {"while", while_},
    // {"or", or_},
    // {"and", and_},
    // {"func", function_},
    // {"while", while_},

    {"==", equal_},
    {"<=", less_than_},
    {">=", more_than_},
    {"=", assign_},
    {"(", lparent_},
    {")", rparent_},
    {"[", lbracket_},
    {"]", rbracket_},

    // {"output", output_},
    // {"input", input_},
    {0, 0},
};

Token *tokens[1000];
extern int tk_pos;

// parsing
int txt_pos;
int line;
char *text;

typedef struct var var;
struct var
{
    char *name;
    union
    {
        double number;
        char *string;
        bool boolean;
    } value;
    int curr_index;
    bool temporary;
    Type type;
};

typedef struct Node Node;
struct Node
{
    Node *left;
    Node *right;
    char *content;
    Type type;
};

int var_index;
var *variables[500];

int tmp_index;
var *temporaries[500];

// functions
char *readline(int fd);
void ft_putnbr(int fd, long num);
void ft_putstr(int fd, char *str);
void ft_printf(int fd, char *fmt, ...);
int ft_strlen(char *str);
void handle_signal(int signum);
void ft_strcpy(char *dest, char *src);
void ft_strncpy(char *dest, char *src, int size);
void print_space(int fd, int line_long, char c);
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
void indexes();
void execute();
char *type_to_string(int type);
void visualize_variables(void);
#endif