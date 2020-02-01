/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dy_tab_add_str.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/23 16:31:13 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 13:20:13 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_dy_tab_add_str(t_dy_tab *d_tab, void *to_add)
{
	if (d_tab->current_size >= d_tab->max_size)
	{
		if (ft_dy_tab_realloc(d_tab))
			return (1);
	}
	if (!(d_tab->tbl[d_tab->current_size] = ft_strdup((char *)to_add)))
		return (1);
	d_tab->current_size++;
	d_tab->tbl[d_tab->current_size] = 0;
	return (0);
}
