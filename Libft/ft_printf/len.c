/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   len.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 10:43:55 by mhrima            #+#    #+#             */
/*   Updated: 2022/11/06 10:43:56 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	len_d(long int num)
{
	int	i;

	i = 0;
	if (!num)
		return (1);
	if (num < 0)
	{
		num = -num;
		i++;
	}
	if (num >= 0 && num < 10)
		i++;
	if (num >= 10)
		i += len_d(num / 10) + 1;
	return (i);
}

int	len_x(unsigned int num)
{
	int	i;

	i = 0;
	if (num >= 0 && num < 16)
		i++;
	if (num >= 16)
		i += len_x(num / 16) + 1;
	return (i);
}

int	len_p(unsigned long long p)
{
	int	i;

	i = 0;
	if (p >= 0 && p < 16)
		i++;
	if (p >= 16)
		i += len_p(p / 16) + 1;
	return (i);
}

int	len_u(unsigned int num)
{
	int	i;

	i = 0;
	if (!num)
		return (1);
	if (num >= 0 && num < 10)
		i++;
	if (num >= 10)
		i += len_u(num / 10) + 1;
	return (i);
}
