/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 23:24:23 by mhrima            #+#    #+#             */
/*   Updated: 2022/12/09 06:04:54 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*the_return(const char *needle, char *str, size_t len)
{
	size_t	i;
	int		j;
	size_t	k;

	i = 0;
	if (!*needle)
		return (str);
	while (str && str[i] && i < len)
	{
		k = i;
		j = 0;
		while (str[k] == needle[j] && needle[j] && str[k] && k < len)
		{
			k++;
			j++;
		}
		if (!needle[j])
			return (&(str[i]));
		i++;
	}
	return (NULL);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char	*str;

	if (ft_strncmp(needle, "", 1) == 0)
		return ((char *)haystack);
	str = (char *)haystack;
	return (the_return(needle, str, len));
}
