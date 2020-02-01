/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 14:26:45 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/19 14:26:47 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	while (n >= 8)
	{
		*(unsigned long *)(dest) = *(unsigned long *)(src);
		dest += 8;
		src += 8;
		n -= 8;
	}
	while (n > 0)
	{
		*(char *)(dest) = *(char *)(src);
		dest++;
		src++;
		n -= 1;
	}
	return (dest);
}
