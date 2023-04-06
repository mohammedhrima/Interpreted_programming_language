#ifndef HEADER_H
#define HEADER_H

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdarg.h>

#define err STDERR_FILENO
#define out STDOUT_FILENO
#define in STDIN_FILENO

typedef enum
{
    INTEGER_TOK,
    PLUS_TOK,
    MINUS_TOK,
    MULTIPLY_TOK,
    DIVIDE_TOK,
    EOF_TOK,
    LPARENT_TOK,
    RPARENT_TOK,
    ERROR_TOK
} Token_type;

typedef struct
{
    int integer;  // used to return
    char *string; // used to return
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
char *ft_strdup(char *str);
char *strjoin(char *string1, char *string2);
char *readline(int fd);
void ft_fprintf(int fd, char *fmt, ...);
// PARSING FUNCTIONS
Token *new_token(Token_type type, char *value_string);
Lexer *new_lexer(char *text);
Interpreter *new_interpreter(char *text);
char *integer(Lexer *self);
int advance(Lexer *self);
Token *get_next_token(Lexer *self);
void eat(Interpreter *self, Token_type token_type);
var *factor(Interpreter *self);
var *term(Interpreter *self);
var *expr(Interpreter *self);

#endif