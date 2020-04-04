/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patch_target.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier </var/mail/ldedier>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 07:03:40 by ldedier           #+#    #+#             */
/*   Updated: 2020/02/23 07:03:40 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int	patch_target_string(unsigned char *data, size_t size, char *pattern, char *to_remplace)
{
	size_t i;
	char *to_compare;
	size_t len;

	i = 0;
	len = ft_strlen(pattern);
	while (i < size - len)
	{
		to_compare = (char *)(data + i);
		if (!ft_strcmp(to_compare, pattern))
		{
			ft_strcpy(to_compare, to_remplace);
			return (0);
		}
		i++;
	}
	return (1);
}

int	patch_target(unsigned char *data, size_t size, long pattern, long to_remplace)
{
	size_t i;
	long *to_compare;
	i = 0;

	while (i < size - sizeof(long))
	{
		to_compare = (long *)(data + i);
		if (*to_compare == pattern)
		{
			*to_compare = to_remplace;
			return (0);
		}
		i++;
	}
	return (1);
}
