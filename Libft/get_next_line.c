/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 05:52:22 by mhrima            #+#    #+#             */
/*   Updated: 2023/02/09 10:41:48 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**split_string(char *str, char c)
{
	int		i;
	char	**arr;

	i = 0;
	arr = my_malloc(2 * sizeof(char *));
	while (str && str[i])
	{
		if (str[i] == c)
		{
			arr[1] = ft_strdup(str + i + 1);
			str[i + 1] = '\0';
			arr[0] = ft_strdup(str);
			return (arr);
		}
		i++;
	}
	return (arr);
}

char	*bcp_with_nl(char **str, char **bcp)
{
	char	*res;
	char	**arr;
	char	*tmp;

	arr = split_string(*bcp, '\n');
	res = arr[0];
	tmp = arr[1];
	free(*bcp);
	*bcp = tmp;
	free(arr);
	free(*str);
	return (res);
}

char	*end_of_file(char **bcp, char **str)
{
	char	*res;

	if (ft_strlen(*bcp))
	{
		free(*str);
		res = ft_strdup(*bcp);
		free(*bcp);
		*bcp = NULL;
		return (res);
	}
	free(*str);
	return (NULL);
}

int	read_from_file(int fd, char **str, char **bcp, int *readen)
{
	char	*tmp;

	ft_bzero(*str, BUFFER_SIZE);
	*readen = read(fd, *str, BUFFER_SIZE * sizeof(char));
	if (*readen > 0)
		(*str)[*readen] = 0;
	if (*readen < 0)
	{
		free(*str);
		return (*readen);
	}
	tmp = ft_strjoin(*bcp, *str);
	free(*bcp);
	*bcp = ft_strdup(tmp);
	free(tmp);
	return (*readen);
}

char	*get_next_line(int fd)
{
	char		*str;
	int			readen;
	static char	*bcp;

	if (fd < 0 || fd > OPEN_MAX)
		return (NULL);
	readen = 1;
	str = my_malloc((BUFFER_SIZE + 1) * sizeof(char));
	while (1)
	{
		if (bcp && ft_strchr(bcp, '\n'))
			return (bcp_with_nl(&str, &bcp));
		if (!readen)
			return (end_of_file(&bcp, &str));
		if (read_from_file(fd, &str, &bcp, &readen) < 0)
			return (NULL);
	}
	free(str);
	return (NULL);
}
