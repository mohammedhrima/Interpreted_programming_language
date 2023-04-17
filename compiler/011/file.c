#include "header.h"

#include "tools.c"
#include "debug.c"

int tk_pos;

void new_token(int start, tok_type type)
{
    tokens[tk_pos] = calloc(1, sizeof(Token));
    tokens[tk_pos]->value = calloc(txt_pos - start + 1, sizeof(char));
    ft_strncpy(tokens[tk_pos]->value, text + start, txt_pos - start);
    tokens[tk_pos]->type = type;
    tokens[++tk_pos] = NULL;
}

void skip_spaces()
{
    while (ft_isspace(text[txt_pos]))
        txt_pos++;
}

void build_tokens()
{
    int start = 0;
    while (text[txt_pos])
    {
        skip_spaces();
        start = txt_pos;
        int type = 0;
        for (int i = 0; multi_tokens[i].type; i++)
        {
            if (ft_strncmp(multi_tokens[i].value, text + txt_pos, ft_strlen(multi_tokens[i].value)) == 0 && ft_isspace(text[txt_pos + ft_strlen(multi_tokens[i].value)]))
            {
                type = multi_tokens[i].type;
                txt_pos += ft_strlen(multi_tokens[i].value);
                break;
            }
        }
        if (type)
            new_token(start, type);
        else if (ft_isalpha(text[txt_pos]))
        {
            // get varibale name
            while (ft_isalnum(text[txt_pos]))
                txt_pos++;
            new_token(start, variable_);
        }
        else if (ft_isdigit(text[txt_pos]))
        {
            // get number value
            while (ft_isdigit(text[txt_pos]))
                txt_pos++;
            new_token(start, number_);
        }
        else if (text[txt_pos] == '"')
        {
            txt_pos++;
            while (text[txt_pos] && text[txt_pos] != '"')
                txt_pos++;
            if (text[txt_pos] != '"')
                ft_printf(err, "Expected '\"'\n");
            txt_pos++;
            new_token(start, characters_);
        }
        else
        {
            ft_printf(err, "unknown value '%s'\n", text + txt_pos);
        }
    }
    new_token(txt_pos, eof_);
}

char *get_type(tok_type type)
{
    switch (type)
    {
    case eof_:
        return "EOF";
    case variable_:
        return "Variable";
    case characters_:
        return "Characters";
    case number_:
        return "Number";
    case assign_:
        return "Assign";
    case equal_:
        return "Equal";
    case less_than_:
        return "Less than";
    case more_than_:
        return "More than";
    case function_:
        return "Function";
    case if_:
        return "if statement";
    case or_:
        return "or operator";
    case and_:
        return "and operator";
    case while_:
        return "while loop";
    case lparent_:
        return "left parant";
    case rparent_:
        return "right parent";
    }
    return NULL;
}

int main(void)
{
    FILE *fp = NULL;
    long file_size = 0;

    // open file
    fp = fopen("file.hr", "r");
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);

    text = calloc(file_size + 1, sizeof(char));
    fseek(fp, 0, SEEK_SET);
    fread(text, file_size, 1, fp);
    fclose(fp);
    ft_printf(out, "%s\n", text);
    indexes();
    ft_printf(out, "\n");

    build_tokens();
    int i = 0;
    while (i < tk_pos)
    {
        char *type_str = get_type(tokens[i]->type);
        if (type_str)
            ft_printf(out, "%s : %s\n", type_str, tokens[i]->value);
        else
            ft_printf(err, "Unknown token '%d'\n", tokens[i]->type);
        i++;
    }
}