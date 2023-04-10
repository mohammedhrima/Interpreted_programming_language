/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 13:35:57 by mhrima            #+#    #+#             */
/*   Updated: 2022/10/13 11:03:55 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	while_loop(int Case, size_t *len, char *pdst, char *psrc)
{
	while (0 < *len)
	{
		*pdst = *psrc;
		if (Case == 1)
		{
			pdst--;
			psrc--;
		}
		if (Case == 2)
		{
			pdst++;
			psrc++;
		}
		(*len)--;
	}
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*pdst;
	char	*psrc;

	if (dst > src)
	{
		pdst = (char *)(dst + len - 1);
		psrc = (char *)(src + len - 1);
		while_loop(1, &len, pdst, psrc);
	}
	if (dst < src)
	{
		pdst = (char *)(dst);
		psrc = (char *)(src);
		while_loop(2, &len, pdst, psrc);
	}
	return (dst);
}
