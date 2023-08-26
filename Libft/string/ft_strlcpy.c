/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:55:02 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/21 07:57:25 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

size_t	ft_strlcpy(char *destination, char *source, size_t size)
{
	unsigned int	j;

	j = 0;
	if (!size)
		return (ft_strlen(source));
	while (j < size - 1 && source[j] != '\0')
	{
		destination[j] = source[j];
		j++;
	}
	destination[j] = '\0';
	return (ft_strlen(source));
}
