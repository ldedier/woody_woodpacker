/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substitute_str.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/30 02:19:35 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/30 05:49:44 by ldedier          ###   ########.fr       */
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

static int	ft_inject_str_in_str(char **str, char *to_inject, int start_index)
{
	char	*tmp;
	int		receiver_len;
	int		to_inject_len;

	to_inject_len = ft_strlen(to_inject);
	receiver_len = ft_strlen(*str);
	if (!(tmp = ft_strnew(to_inject_len + receiver_len)))
		return (1);
	ft_memcpy(tmp, *str, start_index);
	ft_memcpy(&tmp[start_index], to_inject, to_inject_len);
	ft_memcpy(&tmp[start_index + to_inject_len], &((*str)[start_index]),
			receiver_len - start_index);
	free(*str);
	*str = tmp;
	return (0);
}

/*
** realloc str and inject to_inject at the index_to_inject at the place of
** the len character index starting from that index
*/

int			ft_substitute_str(char **str, char *to_inject,
				int index_to_inject, int len)
{
	ft_suppr_n_char_index(*str, index_to_inject, len);
	if (ft_inject_str_in_str(str, to_inject, index_to_inject))
		return (1);
	return (0);
}
