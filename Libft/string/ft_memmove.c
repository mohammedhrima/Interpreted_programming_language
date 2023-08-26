/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:54:37 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/21 07:56:58 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

void	*ft_memmove(void *destination, void *source, size_t len)
{
	unsigned char*	destptr;
	unsigned char*	srcptr;
	size_t				i;

	i = 0;
	destptr	= (unsigned char *)destination;
	srcptr	= (unsigned char *)source;
	while(i < len)
	{
		if(destptr < srcptr)
			destptr[i] = srcptr[i];
		else
			destptr[len - i] = srcptr[len - i];
		i++;
	}
	return (destination);
}