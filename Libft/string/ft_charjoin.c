/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_charjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:54:28 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/21 07:56:50 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

char *ft_charjoin(char *string, char c)
{
    char *res = ft_calloc(ft_strlen(string) + 2, sizeof(char));
    res[ft_strlen(res)] = c;
    free(string);
    return res;
}