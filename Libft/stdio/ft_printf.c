/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 08:14:50 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/22 09:46:38 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdio.h"

void ft_printf(char *conv, ...)
{
    int len = ft_strlen(conv);
    int i;

    va_list args;
    va_start(args, conv);
    i = 0;
    while (i < len)
    {
        if (conv[i] == '%')
        {
            i++;
            if (conv[i] == 'c')
                ft_putchar(1, va_arg(args, int));
            if (conv[i] == 's')
                ft_putstr(1, va_arg(args, char *));
            if (conv[i] == 'p')
            {
                ft_putstr(1, "0x");
                convertfromDec_P(1, (unsigned long long)(va_arg(args, void *)), "0123456789abcdef");
            }
            if (conv[i] == 'x')
                convertfromDec_P(1, (unsigned long long)va_arg(args, void *), "0123456789abcdef");
            if (conv[i] == 'X')
                convertfromDec_P(1, (unsigned long long)va_arg(args, void *), "0123456789ABCDEF");
            if (conv[i] == 'd' || conv[i] == 'i')
                ft_putnbr(1, (long int)va_arg(args, int));
            if (conv[i] == 'f')
                ft_putfloat(1, (double)va_arg(args, int), 10);
            if (conv[i] == 'u')
                ft_putunsignednbr(1, va_arg(args, unsigned));
            if (conv[i] == '%')
                ft_putchar(1, '%');
        }
        else
            ft_putchar(1, conv[i]);
        i++;
    }
    va_end(args);
}