/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:55:13 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/21 07:57:36 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

char	*ft_strrchr(char *string, int c)
{
	int	len;

	len = ft_strlen(string);
	string += len;
	if (c == 0)
		return ((char *)string);
	while (len >= 0)
	{
		if (*string == (char)c)
			return ((char *)string);
		string--;
		len--;
	}
	return (NULL);
}