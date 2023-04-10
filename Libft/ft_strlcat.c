/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 23:00:10 by mhrima            #+#    #+#             */
/*   Updated: 2022/10/13 11:52:21 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, char const *src, size_t size)
{
	size_t	j;
	size_t	len;

	j = 0;
	if (!size || size <= ft_strlen(dst))
		return (ft_strlen(src) + size);
	len = ft_strlen(dst);
	while (j < size - len - 1 && src[j] != '\0')
	{
		dst[len + j] = src[j];
		j++;
	}
	dst[len + j] = '\0';
	return (ft_strlen(src) + len);
}
