/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dy_str_realloc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 14:57:24 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 15:01:30 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_dy_str_realloc(t_dy_str *d_str)
{
	char *tmp;

	if (!(tmp = (char *)ft_memalloc(sizeof(char) *
			((d_str->max_size * 2) + 1))))
		return (1);
	ft_memcpy(tmp, d_str->str, sizeof(char) * d_str->current_size);
	free(d_str->str);
	d_str->str = tmp;
	d_str->max_size *= 2;
	return (0);
}
