/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 22:09:09 by mhrima            #+#    #+#             */
/*   Updated: 2023/01/14 04:23:43 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*currentlst;
	t_list	*nextlst;

	if (!del || !lst)
		return ;
	currentlst = *lst;
	while (currentlst)
	{
		nextlst = currentlst->next;
		my_free(currentlst);
		del(currentlst->content);
		currentlst = nextlst;
	}
	*lst = NULL;
}
