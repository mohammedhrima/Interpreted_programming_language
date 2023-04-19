/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 01:10:09 by mhrima            #+#    #+#             */
/*   Updated: 2023/01/14 02:00:32 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_malloc_free.h"

// my_free_all loop in the linked list
// check if the address savd in node is freed
// if not it frees it
// it work's like ft_lstmap in Libft
//in while loop keep tracking the element of
// nodes and free them if they aren't freed
// then set them as freed
void	my_free_all(void)
{
	t_mal	*var;
	void	*ptr;

	var = *my_allocated_list();
	while (var)
	{
		if (!var->is_free)
		{
			free((void *)(var->address));
			ptr = (void *)(var->address);
			ptr = NULL;
			var->is_free = 1;
			return ;
		}
		var = var->next;
	}
}

// is like my_free_all but it only free the selected pointer
void	my_free(void *ptr)
{
	t_mal			*var;
	unsigned long	address;

	address = (unsigned long)ptr;
	var = *my_allocated_list();
	while (var)
	{
		if (var->address == address && !var->is_free)
		{
			free((void *)(var->address));
			ptr = (void *)(var->address);
			ptr = NULL;
			var->is_free = 1;
			return ;
		}
		var = var->next;
	}
}
