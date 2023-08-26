/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memory.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 08:16:41 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/26 03:28:41 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ft_MEMORY_H__
#define ft_MEMORY_H__

#include "../libft.h"

typedef struct s_mal
{
    int pos;
    uintptr_t *ptr;
    int len;
} t_mal;

void *my_malloc(size_t size);
void	my_free(void *address);
void	my_free_all(void);
#endif