#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

typedef enum
{
    INTEGER_TOK,
    PLUS_TOK,
    MINUS_TOK,
    MULTIPLY_TOK,
    DIVIDE_TOK,
    EOF_TOK
} Token_type;

typedef union
{
    int integer;
    char *string;
} var;

typedef struct
{
    Token_type type;
    char *value;
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
    // char current_char;
} Interpreter;

void putstr(char *str)
{
    int i = 0;
    while (str && str[i])
    {
        write(STDIN_FILENO, str + i, sizeof(char));
        i++;
    }
}

char *readline(int fd)
{
    int len = 0;
    char *res = NULL;

    while (1)
    {
        char c;
        ssize_t n = read(fd, &c, 1);
        if (n <= 0 || c == '\n')
            break;
        char *str = realloc(res, len + 1);
        if (str == NULL)
        {
            free(res);
            return NULL; // out of memory
        }
        res = str;
        res[len] = c;
        len++;
    }
    if (res == NULL || len == 0)
        return NULL; // no input read
    char *str = realloc(res, len + 1);
    if (str == NULL)
    {
        free(res);
        return NULL; // out of memory
    }
    res = str;
    res[len] = '\0'; // terminate string
    return res;
}

Token *new_token(Token_type type, char *value)
{
    Token *new = malloc(sizeof(Token));
    new->value = value;
    new->type = type;
    return (new);
}

Lexer *new_lexer(char *text)
{
    Lexer *new = malloc(sizeof(Lexer));
    new->text = text;
    new->position = 0;
    return (new);
}

Interpreter *new_interpreter(char *text)
{
    Interpreter *new = malloc(sizeof(Interpreter));
    new->lexer = new_lexer(text);
    new->current_token = NULL;
    return (new);
}

void error(char *msg)
{
    putstr(msg);
    putstr("\n");
    exit(-1);
}

void advance(Lexer *self)
{
    if (self->text[self->position])
        self->position++;
}

void skip_whitespace(Lexer *self)
{
    while (self->text && isspace(self->text[self->position]))
        advance(self);
}

char *integer(Lexer *self)
{
    putstr("call integer\n");
    char *res;
    int start = self->position;
    while (self->text[self->position] && isdigit(self->text[self->position]))
        self->position++;
    res = calloc(self->position - start + 1, sizeof(char));
    memcpy(res, self->text + start, self->position - start);
    return (res);
}

Token *get_next_token(Lexer *self)
{
    while (self->text[self->position])
    {
        printf("line 130: => %c\n", self->text[self->position]);
        if (isspace(self->text[self->position]))
        {
            skip_whitespace(self);
            continue;
        }
        if (isdigit(self->text[self->position]))
            return new_token(INTEGER_TOK, integer(self));
        if (self->text[self->position] == '+')
        {
            advance(self);
            return new_token(PLUS_TOK, "+");
        }
        if (self->text[self->position] == '-')
        {
            advance(self);
            return new_token(MINUS_TOK, "-");
        }
        if (self->text[self->position] == '*')
        {
            advance(self);
            return new_token(MULTIPLY_TOK, "*");
        }
        if (self->text[self->position] == '/')
        {
            advance(self);
            return new_token(DIVIDE_TOK, "/");
        }
        // return new_token(EOF_TOK, NULL);
    }
    return new_token(EOF_TOK, NULL);
}

void eat(Interpreter *self, Token_type token_type)
{
    if (self->current_token->type == token_type)
        self->current_token = get_next_token(self->lexer);
    else
        error("Error parsing input");
}

char *factor(Interpreter *self)
{
    Token *token = self->current_token;
    eat(self, INTEGER_TOK);
    return token->value;
}

var *expr(Interpreter *self)
{
    // get first number and move current token to next token
    var *new = calloc(1, sizeof(var));
    if (self == NULL)
        error("error self is null\n");

    self->current_token = get_next_token(self->lexer);
    new->integer = atoi(factor(self));
    while (1)
    {
        switch (self->current_token->type)
        {
        case PLUS_TOK:
            eat(self, PLUS_TOK);
            new->integer += atoi(factor(self));
            // printf("PLUS: res: %d\n", new->integer);
            break;
        case MINUS_TOK:
            eat(self, MINUS_TOK);
            new->integer -= atoi(factor(self));
            // printf("MINUS: res: %d\n", new->integer);
            break;
        case MULTIPLY_TOK:
            eat(self, MULTIPLY_TOK);
            new->integer *= atoi(factor(self));
            // printf("MULTIPLY: res: %d\n", new->integer);
            break;
        case DIVIDE_TOK:
            eat(self, DIVIDE_TOK);
            new->integer /= atoi(factor(self));
            // printf("DEVIDE: res: %d\n", new->integer);
            break;
        default:
            return new;
        }
    }
    return new;
}

int main(void)
{
    while (1)
    {
        putstr("calcul $> ");
        char *text = readline(STDIN_FILENO);
        printf("text = %s\n", text);
        if (text)
        {
            if (text[0] == '\0' || text[0] == '\n')
                break;
            Interpreter *interpreter = new_interpreter(text);
            var *res = expr(interpreter);
            printf("-> %d\n", res->integer);
        }
        free(text);
    }
}