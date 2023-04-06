#include "header.h"

Token *new_token(Token_type type, char *value_string)
{
    Token *new = calloc(1, sizeof(Token));
    new->value = calloc(1, sizeof(var));
    new->value->string = value_string;
    new->type = type;
    return (new);
}

Lexer *new_lexer(char *text)
{
    Lexer *new = calloc(1, sizeof(Lexer));
    new->text = text;
    new->position = 0;
    return (new);
}

Interpreter *new_interpreter(char *text)
{
    Interpreter *new = calloc(1, sizeof(Interpreter));
    // ft_fprintf(out, "new_interpreter with string: %s\n", text);
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
    // ft_strcpy()
    ft_fprintf(out, "line  %d: self.text + start: %s\n", __LINE__, self->text + start);
    memcpy(res, self->text + start, (self->position - start) * sizeof(char));
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
        ft_fprintf(out, "line  %d: %c\n", __LINE__, self->text[self->position]);
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
        else if (self->text[self->position] == '(' && advance(self))
            return new_token(LPARENT_TOK, "(");
        else if (self->text[self->position] == ')' && advance(self))
            return new_token(RPARENT_TOK, ")");
        else
            ft_fprintf(err, "Error in token\n");
    }
    return new_token(EOF_TOK, NULL);
}

void eat(Interpreter *self, Token_type token_type)
{
    if (self->current_token->type == token_type)
        self->current_token = get_next_token(self->lexer);
    else
        ft_fprintf(err, "Error parsing input\n");
}

var *factor(Interpreter *self)
{
    ft_fprintf(out, "line  %d: positon: %d, integer: %d, string: %s\n", __LINE__, self->lexer->position, self->current_token->value->integer, self->lexer->text);
    Token *token = self->current_token;
    if (token->type == INTEGER_TOK)
    {
        eat(self, INTEGER_TOK);
        token->value->integer = atoi(token->value->string);
        return token->value;
    }
    else if (token->type == LPARENT_TOK)
    {
        eat(self, LPARENT_TOK);
        ft_fprintf(out, "line  %d: positon: %d, integer: %d, string: %s\n", __LINE__, self->lexer->position, self->current_token->value->integer, self->lexer->text);
        char *str = ft_strdup(self->lexer->text);
        var *result = expr(self);
        eat(self, RPARENT_TOK);
        return result;
    }
    else if (token->type == RPARENT_TOK)
    {
        ft_fprintf(out, "receive RP\n");
        exit(1);
    }
    // else
    //     ft_fprintf(err, "error in factor, token's type \'%d\'\n", token->type);
    return token->value;
}

var *term(Interpreter *self)
{
    // get first number and move current token to next token
    if (self == NULL)
        ft_fprintf(err, "error self is null in term\n");
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
            break;
    }
    return new;
}

var *expr(Interpreter *self)
{
    // ft_fprintf(out, "handling\'%s\'\n", self->lexer->text[self->lexer->position]);
    if (self == NULL)
        ft_fprintf(err, "error self is null in expr\n");
    self->current_token = get_next_token(self->lexer); // get first token
    var *new = term(self);
    while (1)
    {
        if (self->current_token->type == PLUS_TOK)
        {
            eat(self, PLUS_TOK);
            ft_fprintf(out, "line %d: positon: %d, integer: %d, string: %s\n", __LINE__, self->lexer->position, self->current_token->value->integer, self->lexer->text);
            new->integer += term(self)->integer;
            ft_fprintf(out, "line %d: positon: %d, integer: %d, string: %s\n", __LINE__, self->lexer->position, self->current_token->value->integer, self->lexer->text);
        }
        else if (self->current_token->type == MINUS_TOK)
        {
            eat(self, MINUS_TOK);
            new->integer -= term(self)->integer;
        }
        else
            break;
    }
    return new;
}

int main(void)
{
    while (1)
    {
        ft_fprintf(out, "c_calc $> ");
        char *text = readline(in);
        if (text)
        {
            if (text[0] == '\0' || text[0] == '\n')
                break;
            text[ft_strlen(text) - 1] = '\0'; // replace '\n' with '\0'
            ft_fprintf(out, "text: %s\n", text);
            Interpreter *interpreter = new_interpreter(text);
            var *res = expr(interpreter);
            ft_fprintf(out, "-> %d\n", res->integer);
        }
        free(text);
    }
}