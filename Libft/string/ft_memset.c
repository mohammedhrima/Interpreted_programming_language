/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:54:41 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/21 07:57:00 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

void	*ft_memset(void *pointer, int c, size_t len)
{
	size_t			i;
	unsigned char	*destionation;

	destionation = (unsigned char *)pointer;
	i = 0;
	while (i < len)
	{
		*destionation = (unsigned char)c;
		destionation++;
		i++;
	}
	return (pointer);
}