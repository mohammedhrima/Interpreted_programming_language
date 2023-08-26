/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:55:15 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/21 07:57:39 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

char	*ft_strstr(char *from, char *to_find)
{
	int	i;
	int	k;
	int	j;

	if (!from || !to_find)
		return (NULL);
	i = 0;
	while (from[i])
	{
		k = i;
		j = 0;
		while (from[k] == to_find[j] && to_find[j] && from[k])
		{
			k++;
			j++;
		}
		if (!to_find[j])
			return (from + i);
		i++;
	}
	return (NULL);
}