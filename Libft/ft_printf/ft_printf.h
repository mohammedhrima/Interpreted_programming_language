/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 10:44:50 by mhrima            #+#    #+#             */
/*   Updated: 2022/11/06 10:44:51 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "../libft.h"
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

int		ft_printf(const char *conv, ...);
int		print_space(int *addSpace);
int		ft_putchar(int c);
int		ft_putstr(char *str, int n);
int		ft_putnbr(long int num);
int		ft_putunsignednbr(unsigned int num);
int		len_d(long int num);
int		len_x(unsigned int num);
int		len_p(unsigned long long p);
int		len_u(unsigned int num);
int		convert_from_dec_p(unsigned long long num);
int		convert_from_dec_x(unsigned int num, char x);
int		*set_flag(int *flags, char flag, const char *conv, int *i);
int		*set_flag_arr(int *flags, const char *conv, int *i);
void	reset_flag_arr(int *flags);
void	handle_char(int *flags, int c, int *j);
void	handle_string(int *flags, char *s, int *j);
void	handle_pointer(int *flags, unsigned long long *p, int *j);
void	handle_x(int *flags, unsigned int x, int *j, const char conv);
void	handle_number(int *flags, long int d, int *j);
void	handle_unsigned(int *flags, unsigned int u, int *j);
void	handle_percent(int *flags, char m, int *j);
void	get_argument(const char conv, va_list args, int *flags, int *j);

#endif