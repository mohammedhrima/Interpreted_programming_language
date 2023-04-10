/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 10:44:37 by mhrima            #+#    #+#             */
/*   Updated: 2022/11/06 10:44:38 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(int c)
{
	int	i;

	i = (int)write(1, &c, 1);
	return (i);
}

int	ft_putstr(char *str, int n)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	if (n >= 0)
		len = n;
	else
		len = ft_strlen(str);
	if (!str)
		return (ft_putstr("(null)", -1));
	while (i < len && str[i])
	{
		j += ft_putchar(str[i]);
		if (j < 0)
			return (-1);
		i++;
	}
	return (j);
}

int	ft_putnbr(long int num)
{
	int	i;

	i = 0;
	if (num < 0)
	{
		i += ft_putchar('-');
		num = -num;
	}
	if (num >= 0 && num < 10)
		i += ft_putchar('0' + num);
	if (num >= 10)
	{
		i += ft_putnbr(num / 10);
		i += ft_putchar(num % 10 + '0');
	}
	return (i);
}

int	ft_putunsignednbr(unsigned int num)
{
	int	i;

	i = 0;
	if (num >= 0 && num < 10)
		i += ft_putchar('0' + num);
	if (num >= 10)
	{
		i += ft_putunsignednbr(num / 10);
		i += ft_putchar(num % 10 + '0');
	}
	return (i);
}
