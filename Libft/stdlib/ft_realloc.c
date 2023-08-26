/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:59:28 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/26 03:30:25 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdlib.h"

void	*ft_realloc(void *pointer, size_t oldsize, size_t newsize)
{
	void	*new;

	new = ft_calloc(1, newsize);
	if (pointer)
    {
		ft_memcpy(new, pointer, oldsize);
        // free(pointer);
    }
	return (new);
}