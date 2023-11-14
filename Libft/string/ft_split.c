/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:54:44 by mhrima            #+#    #+#             */
/*   Updated: 2023/11/14 13:14:56 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

char **ft_split(char *string, char *spliter)
{
	int i;
	int j;
	char **res;
	int start;

	i = 0;
	j = 0;
	res = NULL;
	while (string && string[i])
	{
		while (ft_strncmp(string + i, spliter, ft_strlen(spliter)) == 0)
			i += ft_strlen(spliter);
		start = i;
		while (string[i] && ft_strncmp(string + i, spliter, ft_strlen(spliter)))
			i++;
		if (res == NULL)
			res = ft_calloc(j + 2, sizeof(char *));
		else
			res = ft_realloc(res, j * sizeof(char *), (j + 2) * sizeof(char *));
		res[j] = ft_calloc(i - start + 2, sizeof(char));
		ft_strncpy(res[j], string + start, i - start);
		j++;
		res[j] = NULL;
	}
	return (res);
}