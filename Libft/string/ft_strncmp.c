/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:55:08 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/26 02:03:55 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

int	ft_strncmp(char *string1, char *string2, size_t len)
{
    size_t	i;

	i = 0;
	if (!string1)
		return (ft_strlen(string2));
	if (!string2)
		return (ft_strlen(string1));
	while (i < len && string1 && string2 && string1[i] && string1[i] == string2[i])
		i++;
	if(i == len) return 0;
	return (string1[i] - string2[i]);
}