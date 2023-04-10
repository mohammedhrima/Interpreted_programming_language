/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 06:05:07 by mhrima            #+#    #+#             */
/*   Updated: 2022/12/24 05:23:39 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
