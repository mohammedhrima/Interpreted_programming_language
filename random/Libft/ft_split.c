/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 00:31:02 by mhrima            #+#    #+#             */
/*   Updated: 2023/01/14 04:15:10 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	array_len(char const *s, char c)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (s[i])
	{
		while ((s[i] == c))
			i++;
		if (!s[i])
			break ;
		len++;
		while (s[i] != c && s[i])
			i++;
	}
	return (len);
}

void	skip_char_and_get_stringlenght(const char **s, char c, int *j)
{
	while (**s == c)
		(*s)++;
	while (**s != c && **s)
	{
		(*s)++;
		(*j)++;
	}
}

char	**feed_array(char const *s, char c, char **res)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	while (*s)
	{
		j = 0;
		skip_char_and_get_stringlenght(&s, c, &j);
		if (j == 0)
			break ;
		res[k] = (char *)my_malloc((j + 1) * sizeof(char));
		if (res[k] == NULL)
			return (NULL);
		s -= j;
		j = 0;
		while (*s != c && *s)
			res[k][j++] = *(s++);
		res[k][j] = '\0';
		k++;
	}
	res[k] = 0;
	return (res);
}

char	**ft_split(char const *s, char c)
{
	char	**res;

	if (!s)
		return (NULL);
	res = (char **)my_malloc((array_len(s, c) + 1) * sizeof(char *));
	if (res == NULL)
		return (NULL);
	res = feed_array(s, c, res);
	return (res);
}
