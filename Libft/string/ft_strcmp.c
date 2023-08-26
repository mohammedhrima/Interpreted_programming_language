/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:54:49 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/21 07:57:06 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

int	ft_strcmp(char *string1, char *string2)
{
	size_t	i;

	i = 0;
	if (!string1)
		return (ft_strlen(string2));
	if (!string2)
		return (ft_strlen(string1));
	while (string1 && string2 && string1[i] && string1[i] == string2[i])
		i++;
	return (string1[i] - string2[i]);
}