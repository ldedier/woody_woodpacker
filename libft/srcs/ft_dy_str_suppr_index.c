/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dy_str_suppr_index.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 16:00:44 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/18 16:06:43 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_dy_str_suppr_index(t_dy_str *d_str, size_t index)
{
	int i;

	i = index;
	while ((size_t)i < d_str->current_size - 1)
	{
		d_str->str[i] = d_str->str[i + 1];
		i++;
	}
	d_str->str[--d_str->current_size] = 0;
	return (0);
}
