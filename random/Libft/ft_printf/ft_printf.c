/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 10:44:43 by mhrima            #+#    #+#             */
/*   Updated: 2023/01/14 04:24:09 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *conv, ...)
{
	int		i;
	int		j;
	int		*flags;
	va_list	args;

	va_start(args, conv);
	i = 0;
	j = 0;
	flags = (int *)my_malloc(255 * sizeof(int));
	while (i < (int)ft_strlen(conv))
	{
		reset_flag_arr(flags);
		if (conv[i] == '%')
		{
			i++;
			set_flag_arr(flags, conv, &i);
			get_argument(conv[i], args, flags, &j);
		}
		else
			j += ft_putchar(conv[i]);
		i++;
	}
	my_free(flags);
	va_end(args);
	return (j);
}
