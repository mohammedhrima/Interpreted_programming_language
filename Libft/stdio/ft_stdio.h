/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stdio.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 08:14:27 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/26 00:10:23 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STDIO_H__
#define FT_STDIO_H__

#include "../libft.h"
#include <math.h>
void ft_printf(char *conv, ...);
void ft_fprintf(int fd, char *conv, ...);
void ft_putchar(int fd, int c);
void ft_putstr(int fd, char *str);
void ft_putnbr(int fd, long int num);
void ft_putunsignednbr(int fd, unsigned int num);
bool ft_putfloat(int fd, double num, int decimal_places);
void convertfromDec_P(int fd, unsigned long long num, char *to);
void convertfromDec_X(int fd, unsigned int num, char *to);
void print_space(int len);

#endif