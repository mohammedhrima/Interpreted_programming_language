/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_utils_handle_flag.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 10:44:18 by mhrima            #+#    #+#             */
/*   Updated: 2022/11/06 10:47:57 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	*set_flag(int *flags, char flag, const char *conv, int *i)
{
	if (flag == '-')
	{
		flags['0'] = -1;
		while (conv[*i + 1] == '-')
			(*i)++;
	}
	(*i)++;
	flags[(int)flag] = ft_atoi(conv + *i);
	if (conv[*i] >= '1' && conv[*i] <= '9')
		*i += len_d((long)(flags[(int)flag]));
	return (flags);
}

int	*set_flag_arr(int *flags, const char *conv, int *i)
{
	while (conv[*i] && ft_strchr("-+# 0.", conv[*i]))
	{
		if (conv[*i] == '-' || conv[*i] == '0' || conv[*i] == '.')
			flags = set_flag(flags, conv[*i], conv, i);
		else if (conv[*i] == '+')
		{
			flags[' '] = 0;
			flags['+'] = 1;
			(*i)++;
		}
		else if (conv[*i] == '#')
		{
			flags['#'] = 1;
			(*i)++;
		}
		else if (conv[*i] == ' ')
		{
			flags[' '] = 1;
			while (conv[*i] == ' ')
				(*i)++;
		}
	}
	return (flags);
}

void	reset_flag_arr(int *flags)
{
	flags['-'] = -1;
	flags['0'] = -1;
	flags['#'] = 0;
	flags[' '] = 0;
	flags['+'] = 0;
	flags['.'] = -1;
}
