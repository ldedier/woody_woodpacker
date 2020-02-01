/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree_new.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 14:29:27 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/19 14:29:29 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_tree	*ft_tree_new(void *content, size_t size)
{
	t_tree *res;

	if (!(res = (t_tree *)malloc(sizeof(t_tree))))
		return (NULL);
	else
	{
		if (!(res->content = ft_memalloc(size)))
		{
			free(res);
			return (NULL);
		}
		else
			ft_memcpy(res->content, content, size);
		res->left = NULL;
		res->right = NULL;
		return (res);
	}
}
