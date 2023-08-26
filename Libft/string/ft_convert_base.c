/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 08:27:10 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/22 08:28:45 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

char *convert_base(long long number, char *base)
{
    char *converted;
    int index = 0;
    int size = 64;

    converted = ft_calloc(size + 1, sizeof(char));

    if (number == 0)
    {
        converted[index++] = base[0];
    }
    int base_length = ft_strlen(base);
    while (number != 0)
    {
        int remainder = number % base_length;
        converted[index++] = base[remainder];
        number /= base_length;
        if (index >= size)
        {
            converted = ft_realloc(converted, size, 2 * size * sizeof(char));
            size *= 2;
        }
    }
    int start = 0;
    int end = index - 1;
    while (start < end)
    {
        char temp = converted[start];
        converted[start] = converted[end];
        converted[end] = temp;
        start++;
        end--;
    }
    return converted;
}