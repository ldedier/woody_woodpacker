/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dy_tab_cpy_ptr.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 18:24:52 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 13:21:30 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dy_tab	*ft_dy_tab_cpy_ptr(t_dy_tab *d_tab)
{
	t_dy_tab	*res;
	size_t		i;

	if (!(res = ft_dy_tab_new(d_tab->max_size)))
		return (NULL);
	i = 0;
	while (i < d_tab->current_size)
	{
		if (ft_dy_tab_add_ptr(res, d_tab->tbl[i]))
			return (NULL);
		res->current_size = i++ + 1;
	}
	return (res);
}
