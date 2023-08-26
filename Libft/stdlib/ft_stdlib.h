/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stdlib.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 07:57:53 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/22 08:32:39 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STDLIB_H__
#define FT_STDLIB_H__

#include "../libft.h"

int ft_atoi(char *string);
void *ft_calloc(size_t count, size_t size);
char *ft_itoa(int number);
void *ft_realloc(void *pointer, size_t oldsize, size_t newsize);
char *ft_readline(int fd);
#endif