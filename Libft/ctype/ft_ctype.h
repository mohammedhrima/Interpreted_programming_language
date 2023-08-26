/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ctype.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 08:07:06 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/22 12:21:34 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CTYPE_H__
#define FT_CTYPE_H__

#include <stdbool.h>
#include "../libft.h"

bool ft_isalpha(int c);
bool ft_isdigit(int c);
bool ft_isalnum(int c);
bool ft_isascii(int c);
bool ft_isprint(int c);
bool ft_isspace(int c);
bool ft_islower(int c);
bool ft_isupper(int c);
int  to_lower(int c);
int  to_upper(int c);
#endif