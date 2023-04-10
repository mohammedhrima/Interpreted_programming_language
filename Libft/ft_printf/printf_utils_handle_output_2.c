/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_utils_handle_output_2.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 10:44:30 by mhrima            #+#    #+#             */
/*   Updated: 2022/11/06 10:44:31 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	handle_unsigned(int *flags, unsigned int u, int *j)
{
	if (flags['.'] || u)
	{
		if (flags['+'] > 0)
			*j += ft_putchar('+');
		flags['0'] -= len_u(u);
		while (flags['0']-- > 0)
			*j += ft_putchar('0');
		flags['.'] -= len_u(u);
		while (flags['.']-- > 0)
			*j += ft_putchar('0');
		*j += ft_putunsignednbr(u);
		flags['-'] -= len_u(u);
		while (flags['-']-- > 0)
			*j += ft_putchar(' ');
	}
}

void	handle_percent(int *flags, char m, int *j)
{
	*j += ft_putchar(m);
	flags['-'] -= 1;
	while (flags['-']-- > 0)
		*j += ft_putchar(' ');
}

void	get_argument(const char conv, va_list args, int *flags, int *j)
{
	unsigned long long	p;

	if (conv == 'c')
		handle_char(flags, va_arg(args, int), j);
	else if (conv == 's')
		handle_string(flags, va_arg(args, char *), j);
	else if (conv == 'p')
	{
		p = (unsigned long long)va_arg(args, void *);
		handle_pointer(flags, &p, j);
	}
	else if (conv == 'x' || conv == 'X')
		handle_x(flags, (unsigned int)va_arg(args, void *), j, conv);
	else if (conv == 'd' || conv == 'i')
		handle_number(flags, (long int)va_arg(args, int), j);
	else if (conv == 'u')
		handle_unsigned(flags, va_arg(args, unsigned), j);
	else if (conv == '%')
		handle_percent(flags, conv, j);
}
