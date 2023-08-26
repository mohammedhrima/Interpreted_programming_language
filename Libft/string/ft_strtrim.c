/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 08:22:20 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/26 06:06:56 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

char *ft_strtrim(char *str, char *set)
{
    int i = 0;
    int start = 0;
    int end = ft_strlen(str);
    while(str && str[i])
    {
        if(ft_strncmp(str + i, set, ft_strlen(set)) == 0)
        {
            start = i + ft_strlen(set);
            break;
        }
        i++;
    }
    i = ft_strlen(str);
    if(i)
        i -= 1; // protection from segfault
    while(i > start && str && str[i])
    {
        if(ft_strncmp(str + i, set, ft_strlen(set)) == 0)
        {
            end = i;
            break;
        }
        i--;
    }
    char *res = ft_calloc(end - start + 1, sizeof(char));
    ft_strncpy(res  , str + start, end - start);
    return res;
}
