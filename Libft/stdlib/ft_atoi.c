/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:58:30 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/21 07:59:44 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdlib.h"

int	ft_atoi(char *string)
{
	int			i;
	int			sign;
	int			number;

	i = 0;
	sign = 1;
	number = 0;
	while (ft_isspace(string[i]))
		i++;
	if(ft_strchr("+-", string[i]))
	{
		if(string[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(string[i]))
	{
		number = number * 10 + sign * (string[i] - '0');
		if (sign == 1 && ft_isdigit(string[i + 1]) && number > LONG_MAX - sign * (string[i + 1] - '0'))
			return (-1);
		if (sign == -1 && ft_isdigit(string[i + 1]) && number < LONG_MIN - sign * (string[i + 1] - '0'))
			return (0);
		i++;
	}
	return (number);
}
