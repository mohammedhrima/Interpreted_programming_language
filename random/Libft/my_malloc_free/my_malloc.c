/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 01:01:42 by mhrima            #+#    #+#             */
/*   Updated: 2023/01/14 02:01:12 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_malloc_free.h"

// new_var function to allocate t_mal varibale and return it
t_mal	*new_var(unsigned long address)
{
	t_mal	*var;

	var = ft_calloc(1, sizeof(t_mal));
	var->address = address;
	var->next = NULL;
	var->is_free = 0;
	return (var);
}

t_mal	*last_var(t_mal *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

// add_back function look for the last element
// in the node and add the new node to it
void	add_back(t_mal **var, t_mal *new_var)
{
	t_mal	*ptr;

	if (!var)
		return ;
	ptr = last_var(*var);
	if (!ptr)
		*var = new_var;
	else
		ptr->next = new_var;
}

// to be used as global varibales
// I declare (static t_mal	*list) the head of the linked list ,
// when declaring pointer as static,
// it automatically takes NULL as value if (!list)
// first time I call "my_allocate_list",
//'list' varibale will be intialized to null
// that's why I set it using new_var(0) function
// then I return it's pointer
t_mal	**my_allocated_list(void)
{
	static t_mal	*list;

	if (!list)
		list = new_var(0);
	return (&list);
}

// Allocate the needed size and feed it with 0
// then add it to end of the linked list
//"my_allocated_list()"  function is used like a global variable
//"new_var" is a function that allocate t_mal data type
//in the heap and return its value
void	*my_malloc(size_t size)
{
	void	*ptr;

	ptr = ft_calloc(1, size);
	add_back(my_allocated_list(), new_var((unsigned long)ptr));
	return (ptr);
}
