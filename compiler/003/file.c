#include "header.h"




var *new_var(char *name, data type)
{
    static int index;

    variables[index] = calloc(sizeof(var), sizeof(char));
    variables[index]->name = name;
    variables[index]->type = type;
    return variables[index++];
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
    int skiped;
    var *new;
    char *bool_value;
    int operation;

    while (text[pos])
    {

        // skip spaces
        skip_space();
        // get variables
        if (ft_isalpha(text[pos]))
        {
            // get variable name
            start = pos;
            while (ft_isalpha(text[pos]))
                pos++;
            end = pos;
            skip_space();
            // ft_printf(STDOUT, "pos: %d -> %c\n", pos, text[pos]);
            if (text[pos] == '=')
            {
                pos++; // skip =
                skip_space();
                len = end - start;
                new = new_var(NULL, none_);
                // set name
                new->name = calloc(len + 1, sizeof(char));
                ft_strncpy(new->name, text + start, len);
                skip_space();

                // set value and data type
                // chars
                if (text[pos] == '"' || text[pos] == '\'')
                {
                    new->type = characters_;
                    int left_quotes_index = pos++; // save index of first quotes
                    start = pos;
                    while (text[pos] && text[pos] != text[left_quotes_index])
                        pos++;
                    if (text[pos] != text[left_quotes_index])
                        ft_printf(STDERR, "%3d | %0s\n%3d | expecting \'%c\' in index %d\n", line, pos + 1, "^", line + 1, text[left_quotes_index], pos);
                    len = pos - left_quotes_index - 1;
                    new->value.string = calloc(len + 1, sizeof(char));

                    ft_strncpy(new->value.string, text + left_quotes_index + 1, len);
                    return new;
                }
                // number
                else if (ft_isdigit(text[pos]))
                {
                    if (ft_strchr(text + pos, '.'))
                        new->type = float_;
                    else
                        new->type = integer_;
                    new->value.number = ft_atof(text + pos);
                    while (ft_isdigit(text[pos]))
                        pos++;
                    // skip_space();
                    // expect new line !!
                    // if (text[pos] != text[left_quotes_index])
                    //     ft_printf(STDERR, "%3d | %0s\n%3d | expecting \'%c\' in index %d\n", line, pos + 1, "^", line + 1, text[left_quotes_index], pos);
                    return new;
                }
                else if (ft_strncmp(text + pos, "true", ft_strlen("true")) == 0 || ft_strncmp(text + pos, "false", ft_strlen("false")) == 0)
                {
                    // ft_printf(STDOUT, "is boolean\n");
                    // expect new line !!
                    new->type = boolean_;
                    if (ft_strnstr(text + pos, "true", 4))
                    {
                        new->value.boolean = true;
                        pos += 4;
                    }
                    if (ft_strnstr(text + pos, "false", 5))
                    {
                        new->value.boolean = false;
                        pos += 5;
                    }
                    return new;
                }
                else
                    ft_printf(STDERR, "Unknown data type\n");
            }
            else if (ft_strchr("><-+", text[pos]))
            {
                operation = text[pos];
                len = end - start;
                //ft_printf(STDOUT, "token is %c, left is from index %d to %d, len: %d\n", text[pos], start, end - 1, len);
                char *left_name = get_variable_name(left_dir, len);
                pos++;
                char *right_name = get_variable_name(right_dir, 0);
                ft_printf(STDOUT, "compare '%s' and '%s'\n", left_name, right_name);
                var *left_var = get_variable_from_stock(left_name);
                if(left_var == NULL)
                    ft_printf(STDERR, "Unknown variable '%s'\n", left_name);
                var *right_var = get_variable_from_stock(right_name);
                if(right_var == NULL)
                    ft_printf(STDERR, "Unknown variable '%s'\n",right_name);
                new = new_var("tmp",boolean_);
                new->value.boolean = less_than_mor_than(left_var, right_var,operation);
                return (new);
            }
        }
        else
            ft_printf(STDERR, "syntax error\n");
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
                if (variable->type == integer_ || variable->type == float_)
                    ft_printf(STDOUT, "%3d | '%s' is number with value '%f'\n", line, variable->name, variable->value.number);
                if (variable->type == characters_)
                    ft_printf(STDOUT, "%3d | '%s' is characters with value '%s'\n", line, variable->name, variable->value.string);
                if (variable->type == boolean_)
                {
                    if (variable->value.boolean)
                        ft_printf(STDOUT, "%3d | '%s' is boolean with value 'true'\n", line, variable->name);
                    else
                        ft_printf(STDOUT, "%3d | '%s' is boolean with value 'false'\n", line, variable->name);
                }
                line++;
            }
        }
        free(text);
    }
}
