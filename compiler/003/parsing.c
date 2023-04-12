#include "header.h"

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

var* less_than_mor_than(var *left, var *right, int operation)
{
    char *types_stock[10] = {"characters", "integer", "float", "boolean", "none"};
    if (left->type != right->type)
    {
        ft_printf(STDERR, "can't compare type '%s' to '%s'\n", types_stock[left->type], types_stock[right->type]);
        return (false);
    }
    data type = left->type;
    var *temporary = new_var("<>", type, true);
    temporary->type = boolean_;
    if (type == integer_ || type == float_)
    {
        if (operation == '>')
            temporary->value.boolean = left->value.number > right->value.number;
        if (operation == '>')
            temporary->value.boolean = left->value.number < right->value.number; 
    }
    else
        ft_printf(STDOUT, "verify types\n");
    return (temporary);
}

var *math_operation(var *left, var *right, int operation)
{
    char *types_stock[10] = {"characters", "integer", "float", "boolean", "none"};
    if (left->type != right->type)
    {
        ft_printf(STDERR, "can't do '%c' type '%s' to '%s'\n", operation, types_stock[left->type], types_stock[right->type]);
        return (NULL);
    }
    data type = left->type;
    // creat variable with same type as left and right
    var *temporary = new_var("operation", type, true);
    // variable->type = none_;
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
    // else if (type == characters_)
    // {
    //     if (operation == '+')
    //         return left->value.number > right->value.number;
    //     if (operation == '-')
    //         return left->value.number < right->value.number;
    // }
    else
        ft_printf(STDERR, "verify types\n");
    return NULL;
}