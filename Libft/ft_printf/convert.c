/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 10:43:44 by mhrima            #+#    #+#             */
/*   Updated: 2022/11/06 10:43:48 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	convert_from_dec_p(unsigned long long num)
{
	int		i;
	char	*to;

	i = 0;
	to = "0123456789abcdef";
	if (num >= 0 && num < 16)
		i += ft_putchar(to[num]);
	if (num >= 16)
	{
		i += convert_from_dec_p(num / 16);
		i += ft_putchar(to[num % 16]);
	}
	return (i);
}

int	convert_from_dec_x(unsigned int num, char x)
{
	int		i;
	char	*to;

	i = 0;
	if (x == 'x')
		to = "0123456789abcdef";
	if (x == 'X')
		to = "0123456789ABCDEF";
	if (num < 0)
	{
		num *= -1;
		i += ft_putchar('-');
	}
	if (num >= 0 && num < 16)
		i += ft_putchar(to[num]);
	if (num >= 16)
	{
		i += convert_from_dec_x(num / 16, x);
		i += ft_putchar(to[num % 16]);
	}
	return (i);
}
