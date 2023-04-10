#include "header.h"

typedef enum
{
    characters_,
    integer_,
    none_
} data;

typedef struct
{
    char *name;
    union
    {
        int integer;
        char *string;
    } value;
    data type;
} var;

var *new_var(char *name, data type)
{
    var *new = calloc(sizeof(var), sizeof(char));
    new->name = name;
    new->type = type;
    return new;
}

int skip_space()
{
    int skiped_space = 0;
    while (text && isspace(text[pos]))
    {
        pos++;
        skiped_space++;
    }
    return (skiped_space);
}

var *Interpret(char *str)
{
    // skip_space(); // check if add it to variable
    int start = pos;
    int end = pos;
    int len;
    var *new;
    while (text[pos])
    {
        skip_space();
        if (text[pos] == '=')
        {
            // get variable name
            len = pos - start;
            new = new_var(NULL, none_);
            new->name = calloc(pos - start + 1, sizeof(char));
            ft_strncpy(new->name, text + start, len);
            // check variable data type
            pos++;

            if (text[pos] == '"' || text[pos] == '\'')
            {
                new->type = characters_;
                int left_coats_index = pos;
                pos++;
                while (text[pos] && text[pos] != text[left_coats_index])
                    pos++;
                if (text[pos] != text[left_coats_index])
                {
                    ft_printf(STDERR, "%3d | %0s\n%3d | expecting \'%c\' in index %d\n", line, pos + 1, "^", line + 1, text[left_coats_index], pos);
                }
                len = pos - left_coats_index;
                new->value.string = calloc(len + 1, sizeof(char));
                // +1 to sip first coat
                ft_strncpy(new->value.string, text + left_coats_index + 1, len - 1);
                return new;
            }

            return new;
        }
        pos++;
    }
    return NULL;
}

int main(void)
{
    signal(SIGINT, handle_signal);
    while (1)
    {
        pos = 0;
        ft_printf(STDOUT, "%3d | ", line);
        line++;
        text = readline(STDIN);
        if (text)
        {
            if (text[0] == '\n')
                continue;
            text[ft_strlen(text) - 1] = '\0'; // replace \n with \0
            var *variable = Interpret(text);
            if (variable)
            {
                ft_printf(STDOUT, "%3d | %s is %s\n", line, variable->name, variable->value.string);
                line++;
            }
        }
        free(text);
    }
}
