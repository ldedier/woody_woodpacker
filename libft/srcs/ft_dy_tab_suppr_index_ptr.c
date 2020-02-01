/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dy_tab_suppr_index_ptr.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 15:31:18 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 13:20:58 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dy_tab_suppr_index_ptr(t_dy_tab *d_tab, size_t index)
{
	int i;

	i = index;
	while ((size_t)i < d_tab->current_size)
	{
		d_tab->tbl[i] = d_tab->tbl[i + 1];
		i++;
	}
	d_tab->current_size--;
}
