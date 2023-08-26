/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:59:20 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/26 03:27:58 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdlib.h"

void *ft_calloc(size_t count, size_t size)
{
#if 0
	void			*array;

	if (count && size > SIZE_MAX / count)
		return (NULL);
	array = (void *)malloc(count * size);
	if (!array)
	{
        printf("Error allocation in calloc\n");
        exit(ERROR);
    }
	ft_memset(array, 0, count * size);
	return (array);
#endif
	return my_malloc(count * size);
}