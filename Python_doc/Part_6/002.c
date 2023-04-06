#include "header.h"

Token *new_token(Token_type type, char *value_string)
{
    Token *new = malloc(sizeof(Token));
    new->value = malloc(sizeof(var));
    new->value->string = value_string;
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

char *integer(Lexer *self)
{
    char *res;
    int start = self->position;
    while (self->text[self->position] && isdigit(self->text[self->position]))
        self->position++;
    res = calloc(self->position - start + 1, sizeof(char));
    memcpy(res, self->text + start, self->position - start);
    return (res);
}

int advance(Lexer *self)
{
    if (self->text[self->position])
        self->position++;
    return (1);
}

Token *get_next_token(Lexer *self)
{
    while (self->text[self->position])
    {
        ft_fprintf(STDOUT_FILENO, "=> %c\n", self->text[self->position]);
        while (isspace(self->text[self->position]))
            self->position++;
        if (isdigit(self->text[self->position]))
            return new_token(INTEGER_TOK, integer(self));
        else if (self->text[self->position] == '+' && advance(self))
            return new_token(PLUS_TOK, "+");
        else if (self->text[self->position] == '-' && advance(self))
            return new_token(MINUS_TOK, "-");
        else if (self->text[self->position] == '*' && advance(self))
            return new_token(MULTIPLY_TOK, "*");
        else if (self->text[self->position] == '/' && advance(self))
            return new_token(DIVIDE_TOK, "/");
        else
            return new_token(ERROR_TOK, NULL);
    }
    return new_token(EOF_TOK, NULL);
}

void eat(Interpreter *self, Token_type token_type)
{
    if (self->current_token->type == token_type)
        self->current_token = get_next_token(self->lexer);
    else
        ft_fprintf(STDERR_FILENO, "Error parsing input\n");
}

var *factor(Interpreter *self)
{
    Token *token = self->current_token;
    eat(self, INTEGER_TOK);
    token->value->integer = atoi(token->value->string);
    return token->value;
}

var *term(Interpreter *self)
{
    // get first number and move current token to next token
    if (self == NULL)
        ft_fprintf(STDERR_FILENO, "error self is null in term\n");
    var *new = factor(self);
    while (1)
    {
        if (self->current_token->type == MULTIPLY_TOK)
        {
            eat(self, MULTIPLY_TOK);
            new->integer *= factor(self)->integer;
        }
        else if (self->current_token->type == DIVIDE_TOK)
        {
            eat(self, DIVIDE_TOK);
            new->integer /= factor(self)->integer;
        }
        else
            return new;
    }
    return NULL;
}

var *expr(Interpreter *self)
{
    if (self == NULL)
        ft_fprintf(STDERR_FILENO, "error self is null in expr\n");
    self->current_token = get_next_token(self->lexer); // get first token
    var *new = term(self);
    while (1)
    {
        if (self->current_token->type == PLUS_TOK)
        {
            eat(self, PLUS_TOK);
            new->integer += term(self)->integer;
        }
        else if (self->current_token->type == MINUS_TOK)
        {
            eat(self, MINUS_TOK);
            new->integer -= term(self)->integer;
        }
        else
            return new;
    }
    return NULL;
}

int main(void)
{
    while (1)
    {
        ft_fprintf(STDOUT_FILENO, "c_calc> ");
        char *text = readline(STDIN_FILENO);
        ft_fprintf(STDOUT_FILENO, "text: %s", text);
        if (text)
        {
            if (text[0] == '\0' || text[0] == '\n')
                break;
            text[ft_strlen(text) - 1] = '\0'; // replace '\n' with '\0'
            Interpreter *interpreter = new_interpreter(text);
            var *res = expr(interpreter);
            ft_fprintf(STDOUT_FILENO, "-> %d\n", res->integer);
        }
        free(text);
    }
}