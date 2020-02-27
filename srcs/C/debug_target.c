/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_target.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier </var/mail/ldedier>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 07:06:00 by ldedier           #+#    #+#             */
/*   Updated: 2020/02/23 07:06:00 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int	print_some(char *data)
{
	size_t i;
	i = 0;
	while (i < 20)
	{
		printf("%02x\n", data[i]);
		i++;
	}
	return (1);
}

int	scan_target(unsigned char *data, size_t size)
{
	size_t i;
	long *to_compare;
	char *to_compare_c;
	i = 0;

	while (i < size - sizeof(long))
	{
		to_compare = (long *)(data + i);
		ft_printf("%ld\n", *to_compare);
		i++;
	}
	i = 0;
	while (i < size)
	{
		to_compare_c = (char *)(data + i);
		ft_printf("%c", *to_compare_c);
		i++;
	}
	return (1);
}
