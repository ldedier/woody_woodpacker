/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_to_tree_back.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 16:29:09 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/17 17:04:46 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_add_to_tree_back(t_tree **tree, void *content, size_t size)
{
	t_tree *ptr;

	if (*tree == NULL)
	{
		if (!(*tree = ft_tree_new(content, size)))
			return (1);
		else
			return (0);
	}
	ptr = *tree;
	while (ptr->right != NULL)
		ptr = ptr->right;
	if (!(ptr->right = ft_tree_new(content, size)))
		return (1);
	else
		return (0);
}
