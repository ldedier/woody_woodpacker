/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dy_tab_new.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 14:08:58 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 13:20:29 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dy_tab	*ft_dy_tab_new(size_t max_size)
{
	t_dy_tab *res;

	if (!(res = (t_dy_tab *)malloc(sizeof(t_dy_tab))))
		return (NULL);
	res->current_size = 0;
	res->max_size = max_size;
	if (!(res->tbl = (void **)ft_memalloc(max_size * (sizeof(void *) + 1))))
	{
		free(res);
		return (NULL);
	}
	return (res);
}
