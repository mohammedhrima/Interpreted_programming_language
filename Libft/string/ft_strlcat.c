/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:55:00 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/21 07:57:22 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

size_t	ft_strlcat(char *destination, char *source, size_t size)
{
	size_t	j;
	size_t	len;

	j = 0;
	if (!size || size <= ft_strlen(destination))
		return (ft_strlen(source) + size);
	len = ft_strlen(destination);
	while (j < size - len - 1 && source[j] != '\0')
	{
		destination[len + j] = source[j];
		j++;
	}
	destination[len + j] = '\0';
	return (ft_strlen(source) + len);
}