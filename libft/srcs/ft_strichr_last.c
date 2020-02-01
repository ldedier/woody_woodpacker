/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strichr_last.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 19:15:11 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/22 19:15:11 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strichr_last(const char *s, int c)
{
	int i;
	int index;

	index = -1;
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			index = i;
		i++;
	}
	return (index);
}
