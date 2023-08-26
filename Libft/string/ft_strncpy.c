/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:55:10 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/21 07:57:34 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

void ft_strncpy(char *destination, char *source, int size)
{
    int len = ft_strlen(destination);
    int i = 0;
    while (destination && source && source[i] && i < size)
    {
        destination[len + i] = source[i];
        i++;
    }
}