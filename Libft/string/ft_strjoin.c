/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:54:57 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/28 06:54:30 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

char	*ft_strjoin0(char *string1, char *string2)
{
	char	*string;

	if (!string1)
		return (string2);
	if (!string2)
		return (string1);
	string = ft_calloc((ft_strlen(string1) + ft_strlen(string2) + 1), sizeof(char));
	ft_strcpy(string, string1);
	ft_strcpy(string + ft_strlen(string), string2);
	// free(string1);
	// free(string2);
	return (string);
}

char	*ft_strjoin1(char *string1, char *string2)
{
	char	*string;

	if (!string1)
		return (ft_strdup(string2));
	if (!string2)
		return (ft_strdup(string1));
	string = ft_calloc((ft_strlen(string1) + ft_strlen(string2) + 1), sizeof(char));
	ft_strcpy(string, string1);
	ft_strcpy(string + ft_strlen(string), string2);
	return (string);
}

char	*ft_strjoin2(char *string1, char *string2, char *string3)
{
	char	*string;

	string = ft_calloc(ft_strlen(string1) + ft_strlen(string2) + ft_strlen(string3) + 1, sizeof(char));
	if (string1)
		ft_strncpy(string, string1, ft_strlen(string1));
	if (string2)
		ft_strncpy(string + ft_strlen(string), string2, ft_strlen(string2));
	if (string3)
		ft_strncpy(string + ft_strlen(string), string3, ft_strlen(string3));
	return (string);
}
