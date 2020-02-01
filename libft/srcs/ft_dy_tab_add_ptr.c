/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dy_tab_add_ptr.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 14:22:51 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 13:19:53 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_dy_tab_add_ptr(t_dy_tab *d_tab, void *to_add)
{
	if (d_tab->current_size >= d_tab->max_size)
	{
		if (ft_dy_tab_realloc(d_tab))
			return (1);
	}
	d_tab->tbl[d_tab->current_size++] = to_add;
	d_tab->tbl[d_tab->current_size] = 0;
	return (0);
}
