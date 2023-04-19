/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_malloc_free.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 19:27:00 by mhrima            #+#    #+#             */
/*   Updated: 2023/01/14 01:20:30 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MY_MALLOC_FREE_H
# define MY_MALLOC_FREE_H

# include "../libft.h"
# include "limits.h"
# include "stdlib.h"
# include "unistd.h"

typedef struct s_mal
{
	unsigned long	address;
	struct s_mal	*next;
	int				is_free;
}					t_mal;

t_mal				*new_var(unsigned long address);
t_mal				*last_var(t_mal *lst);
void				add_back(t_mal **var, t_mal *new_var);
t_mal				**my_allocated_list(void);
void				*my_malloc(size_t size);
void				my_free_all(void);
void				my_free(void *ptr);

#endif