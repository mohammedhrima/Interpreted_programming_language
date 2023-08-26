/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 08:43:01 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/22 09:20:41 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdio.h"

void ft_putchar(int fd, int c)
{
    write(fd, &c, 1);
}

void ft_putstr(int fd, char *str)
{
    if (!str)
        return (ft_putstr(fd ,"(null)"));
    write(fd, str, ft_strlen(str));
}

void ft_putnbr(int fd, long int num)
{
    if (num < 0)
    {
        ft_putchar(fd, '-');
        num = -num;
    }
    if (num >= 0 && num < 10)
        ft_putchar(fd, '0' + num);
    if (num >= 10)
    {
        ft_putnbr(fd, num / 10);
        ft_putchar(fd, num % 10 + '0');
    }
}

void ft_putunsignednbr(int fd, unsigned int num)
{
    if (num < 10)
        ft_putchar(fd, '0' + num);
    if (num >= 10)
    {
        ft_putunsignednbr(fd, num / 10);
        ft_putchar(fd, num % 10 + '0');
    }
}

void convertfromDec_P(int fd, unsigned long long num, char *to)
{
    unsigned long long len = (unsigned long long)ft_strlen(to);

    if (num < len)
        ft_putchar(fd, to[num]);
    if (num >= len)
    {
        convertfromDec_P(fd, num / len, to);
        ft_putchar(fd, to[num % len]);
    }
}

void convertfromDec_X(int fd, unsigned int num, char *to)
{
    unsigned int len = (unsigned)ft_strlen(to);

    if (num < len)
        ft_putchar(fd, to[num]);
    if (num >= len)
    {
        convertfromDec_X(fd, num / len, to);
        ft_putchar(fd, to[num % len]);
    }
}

bool ft_putfloat(int fd, double num, int decimal_places)
{
    bool is_float = true;
    if (num < 0.0)
    {
        ft_putchar(fd, '-');
        num = -num;
    }
    long int_part = (long)num;
    double float_part = num - (double)int_part;
    while (decimal_places > 0)
    {
        float_part *= 10;
        decimal_places--;
    }
    ft_putnbr(fd, int_part);
    if (decimal_places)
    {
        ft_putchar(fd, '.');
        ft_putnbr(fd, (long)round(float_part));
    }
    else
        is_float = false;
    return is_float;
}

void print_space(int len)
{
    int i = 0;
    while (i < len)
    {
        ft_putchar(out, ' ');
        i++;
    }
}
