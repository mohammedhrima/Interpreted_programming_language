/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 00:03:19 by mhrima            #+#    #+#             */
/*   Updated: 2023/01/14 04:15:35 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	includes(char const *set, char c)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	get_start(char const *set, char const *s1)
{
	size_t	i;

	i = 0;
	while (includes(set, s1[i]))
		i++;
	return (i);
}

int	get_end(char const *set, char const *s1)
{
	size_t	i;

	i = ft_strlen(s1) - 1;
	while (includes(set, s1[i]))
		i--;
	return (i);
}

char	*the_return(int start, int end, const char *s1)
{
	char	*res;
	size_t	i;

	i = 0;
	res = (char *)my_malloc((end - start + 2) * sizeof(char));
	if (res == NULL)
		return (NULL);
	while (i < (unsigned)end - (unsigned)start + 1)
	{
		res[i] = s1[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int	start;
	int	end;

	if (!s1)
		return (NULL);
	if (!set)
		return ((char *)s1);
	start = get_start(set, s1);
	end = get_end(set, s1);
	if (start > end)
		return (ft_calloc(1, 1));
	return (the_return(start, end, s1));
}
