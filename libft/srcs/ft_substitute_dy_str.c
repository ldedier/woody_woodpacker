/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substitute_dy_str.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/21 16:54:15 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/26 23:16:36 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_suppr_n_char_index(char *str, int index, int len)
{
	int i;

	i = index;
	while (str[i + len])
	{
		str[i] = str[i + len];
		i++;
	}
	str[i] = '\0';
}

static void	ft_inject_str_in_str(char *str, char *to_inject, int start_index,
			int to_inject_len)
{
	int		i;
	int		j;
	int		receiver_len;

	receiver_len = ft_strlen(str);
	i = receiver_len;
	while (i > start_index)
	{
		str[i + to_inject_len] = str[i];
		i--;
	}
	j = 0;
	while (j < to_inject_len)
	{
		str[i + j] = to_inject[j];
		j++;
	}
}

/*
** realloc d_str and inject to_inject at the index_to_inject at the place of
** the len characters starting from that index
*/

int			ft_substitute_dy_str(t_dy_str *d_str, char *to_inject,
				int index_to_inject, int len)
{
	int necessary_size;
	int to_inject_len;

	to_inject_len = ft_strlen(to_inject);
	necessary_size = d_str->current_size - len + to_inject_len;
	while ((int)d_str->max_size < necessary_size)
	{
		if (ft_dy_str_realloc(d_str))
			return (1);
	}
	ft_suppr_n_char_index(d_str->str, index_to_inject, len);
	ft_inject_str_in_str(d_str->str, to_inject,
			index_to_inject, to_inject_len);
	d_str->current_size = necessary_size;
	return (0);
}
