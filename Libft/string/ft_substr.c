/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:55:18 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/26 05:56:26 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

char	*ft_substr(char *string, unsigned int start, size_t len)
{
	char			*ptr;
	char			*s1;
	unsigned int	i;

	if (!string || start >= ft_strlen(string))
		return (NULL);
	if (len > ft_strlen(string) - start)
		len = ft_strlen(string) - start;
	ptr = (char *)ft_calloc(len + 1, sizeof(char));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	s1 = ft_strdup(string);
	while (i < len && string[start])
	{
		ptr[i] = s1[start];
		start++;
		i++;
	}
	// free(s1);
	ptr[i] = '\0';
	return (ptr);
}