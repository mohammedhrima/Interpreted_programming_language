#include "header.h"

void indexes()
{
    int i = 0;
    ft_printf(out, "");
    while (text && text[i])
    {
        ft_putnbr(out, i % 10);
        i++;
    }
    ft_putstr(out, "\n");
}

void visualize_variables(void)
{
    // pos += ft_strlen("vars");
    int i = 0;
    ft_printf(out, "permanent variables: \n");
    while (variables[i])
    {
        var *variable = variables[i];
        if (variable)
            ft_printf(out, "     %v\n", variable);
        i++;
    }
    i = 0;
    ft_printf(out, "temporary variables: \n");
    while (temporaries[i])
    {
        var *temporary = temporaries[i];
        if (temporary)
            ft_printf(out, "     %v\n", temporary);
        i++;
    }
}