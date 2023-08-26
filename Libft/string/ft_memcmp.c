/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:54:31 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/21 07:56:53 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

int	ft_memcmp(void *pointer1, void *pointer2, size_t len)
{
	size_t	i;
	unsigned char *string1;
	unsigned char *string2;

	string1 = (unsigned char *)pointer1;
	string2 = (unsigned char *)pointer2;
	if(!string1 && string2)
		return string2[0];
	if(!string2 && string1)
		return string1[0];
	i = 0;
	while (i < len && string1[i] && string1[i] == string2[i])
		i++;
	return (string1[i] - string2[i]);
}