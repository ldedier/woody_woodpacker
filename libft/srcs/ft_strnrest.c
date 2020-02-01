/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnrest.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 20:47:06 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/14 16:34:17 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnrest(char *str, int n)
{
	char	*res;
	int		len;

	len = ft_strlen(str);
	if (!(res = ft_strndup(&str[n], len - n)))
		return (NULL);
	return (res);
}
