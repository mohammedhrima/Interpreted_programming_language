#include "header.h"

char *get_variable_name(direction dir, int len)
{
    char *name = NULL;
    int start = 0;
    int end = 0;
    if (dir == left_dir)
    {
       // ft_printf(STDOUT, "pos: %d , len: %d, sub: %d   \n", pos, len, pos - len);
        int start = pos - len;
        //ft_printf(STDOUT, "tmp_pos: %d\n", start);
        name = calloc(len + 1, sizeof(char));
        ft_strncpy(name, &text[start], len);
    }
    else if (dir == right_dir)
    {
        start = pos;
        end = start;
        while (ft_isalpha(text[end]))
            end++;
        //ft_printf(STDOUT, "copy from %d to %d\n", start, end);
        name = calloc(end - start + 1, sizeof(char));
        ft_strncpy(name, text + start, end - start);
    }
    else
        ft_printf(STDERR, "verify direction\n");

    return name;
}

var *get_variable_from_stock(char *name)
{
    int i = 0;
    while (variables[i])
    {
        if (!ft_strcmp(name, variables[i]->name))
            return variables[i];
        i++;
    }
    return NULL;
}

bool less_than_mor_than(var *left, var *right, int operation)
{
    char *types_stock[10] = {"characters", "integer", "float", "boolean", "none"};
    if (left->type != right->type)
    {
        ft_printf(STDERR, "can't compare type '%s' to '%s'\n", types_stock[left->type], types_stock[right->type]);
        return (false);
    }
    data type = left->type;
    if (type == integer_ || type == float_)
    {
        if (operation == '>')
            return left->value.number > right->value.number;
        if (operation == '>')
            return left->value.number < right->value.number;
    }
    else
        ft_printf(STDOUT,"verify types\n");
    return (3);
}
