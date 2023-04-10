#include "header.h"

typedef enum
{
    string_,
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

var *new_var(char *name)
{
    var *new = calloc(sizeof(var), sizeof(char));
    new->name = name;
    return new;
}

int skip_space()
{
    int skiped_space = 0;
    while (text && isspace(text[column]))
    {
        column++;
        skiped_space++;
    }
    return (skiped_space);
}

// char *get_name()
// {

// }

var *Interpret(char *str)
{
    // skip_space(); // check if add it to variable
    int start = column;
    int end = column;
    while (text[column])
    {
        skip_space();
        if (text[column] == '=')
        {
            // get variable name
            int len = column - start;
            data type = none_;
            // ft_printf(STDOUT, "copy %d from %s \n", len, text + start);
            var *new = new_var(NULL);
            new->name = calloc(column - start + 1, sizeof(char));
            ft_strncpy(new->name, text + start, len);
            /// move start
            start = column + 1;
            // get variable value
            // start = column++;
            len = ft_strlen(text) - start;
            new->value.string = calloc(len + 1, sizeof(char));
            ft_strncpy(new->value.string, text + start, len);
            return new;
        }
        column++;
    }
    return NULL;
}

int main(void)
{
    signal(SIGINT, handle_signal);
    int line = 0;
    while (1)
    {
        line++;
        column = 0;
        ft_printf(STDOUT, "%3d | ", line);
        text = readline(STDIN);
        if (text)
        {
            if(text[0] =='\n')
                continue;
            line++;
            text[ft_strlen(text) - 1] = '\0'; // replace \n with \0
            // ft_printf(STDOUT, "%3d | received: %s\n", line, text);
            line++;
            var *variable = Interpret(text);
            if(variable)
                ft_printf(STDOUT, "%3d | %s=%s\n", line, variable->name, variable->value);
        }
        free(text);
    }
}
