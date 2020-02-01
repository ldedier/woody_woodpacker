/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dy_tab_cpy_str.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 18:46:14 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 13:21:42 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dy_tab	*ft_dy_tab_cpy_str(t_dy_tab *d_tab)
{
	t_dy_tab	*res;
	size_t		i;
	char		*str;

	if (!(res = ft_dy_tab_new(d_tab->max_size)))
		return (NULL);
	i = 0;
	while (i < d_tab->current_size)
	{
		if (!(str = ft_strdup((char *)d_tab->tbl[i])))
		{
			ft_dy_tab_del(res);
			return (NULL);
		}
		if (ft_dy_tab_add_ptr(res, str))
		{
			ft_dy_tab_del(res);
			return (NULL);
		}
		res->current_size = i++ + 1;
	}
	return (res);
}
