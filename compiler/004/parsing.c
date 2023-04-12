#include "header.h"

char *to_string(data type)
{
    char *types_stock[10] = {"characters", "integer", "float", "boolean", "none"};
    return (types_stock[type]);
}

// void print_variabel(var * variable)
// {
//     ft_printf(STDOUT)
// }

void assign_var(var *dest, var *src)
{
    if (dest->type != none_ && dest->type != src->type)
        ft_printf(STDERR, "can't assign '%s' type '%s' to '%s' type '%s'\n", src->name, to_string(src->type), dest->name, to_string(dest->type));
    else
    {
        int dest_index = dest->curr_index;
        char *dest_name = dest->name;

        ft_memcpy(dest, src, sizeof(var));
        dest->curr_index = dest_index;
        dest->name = dest_name;
    }
    ft_printf(STDOUT, "conflit in data type between '%s' type '%s' and '%s' type '%s'\n", src->name, to_string(src->type), dest->name, to_string(dest->type));
}

char *get_variable_name(int start, int end)
{
    char *name = calloc(end - start + 1, sizeof(char));
    ft_strncpy(name, text + start, end - start);
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

// return boolean
var *less_than_more_than(var *left, var *right, int operation)
{
    // char *types_stock[10] = {"characters", "integer", "float", "boolean", "none"};
    if (left->type != right->type)
    {
        ft_printf(STDERR, "can't compare type '%s' to '%s'\n", to_string(left->type), to_string(right->type));
        return (false);
    }
    data type = left->type;
    var *temporary = new_var("<>", boolean_, true);
    if (type == integer_ || type == float_)
    {
        bool res = false;
        if (operation == '>')
        {
            res = left->value.number > right->value.number;
            temporary->value.boolean = left->value.number > right->value.number;
            // ft_printf(STDOUT, "do > res: %d\n", res);
        }
        if (operation == '<')
        {
            res = left->value.number < right->value.number;
            temporary->value.boolean = left->value.number < right->value.number;
            // ft_printf(STDOUT, "do < res: %d\n", res);
        }
    }
    else
        ft_printf(STDOUT, "verify types\n");
    return (temporary);
}

var *math_operation(var *left, var *right, int operation)
{
    // char *types_stock[10] = {"characters", "integer", "float", "boolean", "none"};
    if (left->type != right->type)
    {
        ft_printf(STDERR, "can't do '%c' type '%s' to '%s'\n", operation, to_string(left->type), to_string(right->type));
        return (NULL);
    }
    data type = left->type;
    // create variable with same type as left and right
    var *temporary = new_var("operation", type, true);
    if (type == integer_ || type == float_)
    {
        if (operation == '+')
        {
            temporary->type = type;
            temporary->value.number = left->value.number + right->value.number;
            return temporary;
        }
        if (operation == '-')
        {
            temporary->type = type;
            temporary->value.number = left->value.number - right->value.number;
            return temporary;
        }
    }
    else if (type == characters_)
    {
        if (operation == '+')
        {
            char *joined_string = calloc(ft_strlen(left->value.string) + ft_strlen(right->value.string) + 1, sizeof(char));
            ft_strcpy(joined_string + ft_strlen(joined_string), left->value.string );
            ft_strcpy(joined_string + ft_strlen(joined_string), right->value.string );
            temporary->value.string = joined_string;
        }
        if (operation == '-')
        {
            temporary->type = none_;
            ft_printf(STDERR, "can't substract '%s' from '%s'\n", to_string(type), to_string(type));
        }
    }
    else
        ft_printf(STDERR, "verify types\n");
    return temporary;
}

void visualize_variables(void)
{
    int i = 0;
    ft_printf(STDOUT, "permanent variables: \n");
    while (variables[i])
    {
        var *variable = variables[i];
        if (variable)
            ft_printf(STDOUT, "     %v\n", variable);
        i++;
    }
    i = 0;
    ft_printf(STDOUT, "temporary variables: \n");
    while (temporaries[i])
    {
        var *temporary = variables[i];
        if (temporary)
            ft_printf(STDOUT, "     %v\n", temporary);
        i++;
    }
}