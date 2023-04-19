/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_utils_handle_output_1.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 10:44:24 by mhrima            #+#    #+#             */
/*   Updated: 2022/11/06 10:44:26 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	handle_char(int *flags, int c, int *j)
{
	*j += ft_putchar(c);
	flags['-'] -= 1;
	while (flags['-']-- > 0)
		*j += ft_putchar(' ');
}

void	handle_string(int *flags, char *s, int *j)
{
	if (flags['.'] || s)
	{
		*j += ft_putstr(s, flags['.']);
		flags['-'] -= ft_strlen(s);
		while (flags['-']-- > 0)
			*j += ft_putchar(' ');
	}
}

void	handle_pointer(int *flags, unsigned long long *p, int *j)
{
	*j += ft_putstr("0x", -1) + convert_from_dec_p(*p);
	flags['-'] -= (len_p(*p) + 2);
	while (flags['-']-- > 0)
		*j += ft_putchar(' ');
}

void	handle_x(int *flags, unsigned int x, int *j, const char conv)
{
	if (flags['#'] && x)
	{
		*j += ft_putchar('0');
		*j += ft_putchar(conv);
	}
	flags['.'] -= len_x(x);
	while (flags['.']-- > 0)
		*j += ft_putchar('0');
	flags['0'] -= len_x(x);
	while (flags['0']-- > 0)
		*j += ft_putchar('0');
	*j += convert_from_dec_x(x, conv);
	flags['-'] -= len_x(x);
	while (flags['-']-- > 0)
		*j += ft_putchar(' ');
}

void	handle_number(int *flags, long int d, int *j)
{
	if (flags['.'] || d)
	{
		flags['-'] -= len_d(d);
		if (d >= 0 && flags[' '])
			*j += ft_putchar(' ');
		if (flags['+'] > 0 && d >= 0)
			*j += ft_putchar('+');
		if (d < 0)
		{
			*j += ft_putchar('-');
			d = -d;
			flags['0']--;
		}
		flags['0'] -= len_d(d);
		while (flags['0']-- > 0)
			*j += ft_putchar('0');
		flags['.'] -= len_d(d);
		while (flags['.']-- > 0)
			*j += ft_putchar('0');
		*j += ft_putnbr(d);
		while (flags['-']-- > 0)
			*j += ft_putchar(' ');
	}
}
