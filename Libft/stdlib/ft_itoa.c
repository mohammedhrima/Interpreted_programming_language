/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:59:09 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/21 07:59:10 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdlib.h"

char	*ft_itoa(int number)
{
	char	*string;
    bool    is_negative;
	size_t		i;
	char	c;
	
	i = 0;
    is_negative = false;
	if (number == -2147483648)
		return (ft_strdup("-2147483648"));
	if(number == 0)
		return (ft_strdup("0"));
	string = NULL;
	if (number < 0)
	{
        is_negative = true;
		number = -number;
	}
	while(number > 0)
	{
		string = ft_charjoin(string, number % 10);
		number /= 10;
	}
    if(is_negative)
		string = ft_strjoin0(string, ft_strdup("-"));
	i = 0;
	while(i < ft_strlen(string) / 2)
	{
        c = string[i];
		string[i] = string[ft_strlen(string) - i - 1];
        string[ft_strlen(string) - i - 1] = c;
		i++;
	}
	return (string);
}