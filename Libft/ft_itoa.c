/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 10:13:05 by mhrima            #+#    #+#             */
/*   Updated: 2023/01/14 04:14:48 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	lvl(int num)
{
	int	i;

	i = 0;
	if (num >= 0 && num < 10)
		return (1);
	if (num / 10 > 0)
		i++;
	while (num / 10 > 0)
	{
		num /= 10;
		i++;
	}
	return (i);
}

char	*create_int(int len, int n, char *res, int neg)
{
	while (len >= neg)
	{
		res[len] = n % 10 + '0';
		n /= 10;
		len--;
	}
	return (res);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		len;
	int		neg;

	neg = 0;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n < 0)
	{
		neg = 1;
		n = -n;
	}
	len = lvl(n);
	if (neg)
		len++;
	res = (char *)my_malloc((len + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	res[len] = '\0';
	if (neg)
		res[0] = '-';
	return (create_int(len - 1, n, res, neg));
}
