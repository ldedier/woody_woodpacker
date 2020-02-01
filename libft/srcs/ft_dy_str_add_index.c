/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dy_str_add_index.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 16:19:55 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 16:20:02 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_dy_str_add_index(t_dy_str *d_str, char c, size_t index)
{
	int i;

	if (d_str->current_size >= d_str->max_size)
	{
		if (ft_dy_str_realloc(d_str))
			return (1);
	}
	i = d_str->current_size;
	while ((size_t)i > index)
	{
		d_str->str[i] = d_str->str[i - 1];
		i--;
	}
	d_str->str[i] = c;
	d_str->str[++d_str->current_size] = 0;
	return (0);
}
