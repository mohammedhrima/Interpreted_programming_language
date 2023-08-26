/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:54:52 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/21 07:57:09 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

void ft_strcpy(char *destination, char *source)
{
    int len = ft_strlen(destination);
    int i = 0;
    while (destination && source && source[i])
    {
        destination[len + i] = source[i];
        i++;
    }
}