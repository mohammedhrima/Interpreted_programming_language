#include "header.h"

/*==============================Tokenizing==============================*/
// text
char *text = NULL;
int txt_pos = 0;

// tokens
Token **tokens = NULL;
int tk_len = 50;
int tk_pos = 0;

// position in file
int line = 1;
int column = 0;
int tab = 0;
int start = 0;

// tab space
char *tab_space = "    ";

Token *new_token(Type type)
{
    Token *new = ft_calloc(1, sizeof(Token));
    new->type = type;
    new->line = line;
    new->column = txt_pos - column;
    new->txt_pos = txt_pos;
    new->tab = tab;
    switch (type)
    {
    case identifier_:
    {
        type = 0;
        char *value = ft_calloc(txt_pos - start + 1, sizeof(char));
        ft_strncpy(value, text + start, txt_pos - start);
        for (int i = 0; alpha_tokens[i].name; i++)
            if (ft_strcmp(value, alpha_tokens[i].name) == 0)
            {
                type = alpha_tokens[i].type;
                break;
            }
        if (type)
        {
            new->type = type;
            if (type == boolean_)
            {
                if (ft_strcmp(value, "true") == 0)
                    new->boolean = true;
                else if (ft_strcmp(value, "false") == 0)
                    new->boolean = false;
            }
            // free(value);
        }
        else
            new->name = value;
        break;
    }
    case characters_:
    {
        new->characters = ft_calloc(txt_pos - start + 1, sizeof(char));
        ft_strncpy(new->characters, text + start, txt_pos - start);
        break;
    }
    case number_:
    {
        long long num = 0;
        int exponent = 0;
        int after_decimal = 0; // Flag to check if we're after a decimal point

        while (ft_isdigit(text[start]) || text[start] == '.')
        {
            if (text[start] == '.')
            {
                after_decimal = 1;
                start++;
                continue;
            }

            num = 10 * num + text[start] - '0';
            start++;

            if (after_decimal)
            {
                exponent++;
            }
        }
        new->number = num;
        new->exponent = exponent;
        // printf("-> %llu\n", new->number);
        // printf("-> %d\n", new->exponent);
        // exit(0);
        break;
    }
    default:
        break;
    }
    if (tokens == NULL)
        tokens = ft_calloc(tk_len, sizeof(Token));
    if (tk_pos + 10 == tk_len)
    {
        tokens = ft_realloc(tokens, tk_len * sizeof(Token *), 2 * tk_len * sizeof(Token *));
        tk_len *= 2;
    }
    tokens[tk_pos] = new;
    tk_pos++;
    tokens[tk_pos] = NULL;
    see_token(new);
    start = 0;
    return new;
}

void build_tokens()
{
    column = txt_pos;
    while (text[txt_pos])
    {
        if (ft_strncmp(text + txt_pos, tab_space, ft_strlen(tab_space)) == 0)
        {
            txt_pos += ft_strlen(tab_space);
            tab++;
            continue;
        }
        if (ft_isspace(text[txt_pos]))
        {
            if (text[txt_pos] == '\n')
            {
                line++;
                tab = 0;
                column = txt_pos + 1;
            }
            txt_pos++;
            continue;
        }
        // skip comments /* */
        if (ft_strncmp(text + txt_pos, "/*", ft_strlen("/*")) == 0)
        {
            txt_pos += ft_strlen("/*");
            while (text[txt_pos] && ft_strncmp(text + txt_pos, "*/", ft_strlen("*/")))
                txt_pos++;
            txt_pos += ft_strlen("*/");
            continue;
        }
        // // skip comments /
        if (ft_strncmp(&text[txt_pos], "//", ft_strlen("//")) == 0)
        {
            while (text[txt_pos] && text[txt_pos] != '\n')
                txt_pos++;
            continue;
        }
        int type = 0;
        for (int i = 0; operators_tokens[i].name; i++)
        {
            if (ft_strncmp(operators_tokens[i].name, text + txt_pos, ft_strlen(operators_tokens[i].name)) == 0)
            {
                start = txt_pos;
                type = operators_tokens[i].type;
                txt_pos += ft_strlen(operators_tokens[i].name);
                new_token(type);
                break;
            }
        }
        if (type)
        {
            if (type == dots_)
                tab++; // for if statements and while loops ...
            continue;
        }
        if (ft_isalpha(text[txt_pos]) || text[txt_pos] == '_')
        {
            start = txt_pos;
            while (ft_isalnum(text[txt_pos]) || text[txt_pos] == '_')
                txt_pos++;
            Token *token = new_token(identifier_);
            if (token->type == return_)
                tab++;
            continue;
        }
        if (ft_isdigit(text[txt_pos]))
        {
            start = txt_pos;
            while (ft_isdigit(text[txt_pos]))
                txt_pos++;
            if (text[txt_pos] == '.')
                txt_pos++;
            while (ft_isdigit(text[txt_pos]))
                txt_pos++;
            new_token(number_);
            continue;
        }
        if (text[txt_pos] == '"' || text[txt_pos] == '\'')
        {
            start = txt_pos;
            txt_pos++;
            // ft_putchar(out, '\n');
            while (text[txt_pos] && text[txt_pos] != text[start])
                txt_pos++;
            if (text[txt_pos] != text[start])
            {
                ft_putchar(out, '\n');
                while (txt_pos > 0 && text[txt_pos - 1] != '\n')
                    txt_pos--;
                int i = txt_pos;
                while (text[txt_pos] && text[txt_pos] != '\n')
                {
                    ft_putchar(out, text[txt_pos]);
                    txt_pos++;
                }
                ft_putchar(out, '\n');
                print_space(txt_pos - i);
                ft_putstr(out, "^\n");
                ft_fprintf(err, "Syntax error expected '%c'\n", text[start]);
            }
            start++;
            new_token(characters_);
            txt_pos++;
            continue;
        }
        if (text[txt_pos])
        {
            ft_printf("found this '%c'\n", text[txt_pos]);
            ft_putchar(out, '\n');
            while (txt_pos > 0 && text[txt_pos - 1] != '\n')
                txt_pos--;
            int i = txt_pos;
            while (text[txt_pos] && text[txt_pos] != '\n')
            {
                ft_putchar(out, text[txt_pos]);
                txt_pos++;
            }
            ft_putchar(out, '\n');
            print_space(txt_pos - i);
            ft_putstr(out, "^\n");
            ft_fprintf(err, "Syntax error\n");
        }
    }
    new_token(eof_);
}