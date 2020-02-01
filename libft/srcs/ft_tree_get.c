/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree_get.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/23 03:40:55 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/23 03:40:55 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_tree	*ft_tree_get(t_tree *tree, void *ref,
			int (*identify)(void*, void *))
{
	void *res;

	if (tree == NULL)
		return (NULL);
	else
	{
		if (!(identify(tree->content, ref)))
			return (tree);
		else
			return ((res = ft_tree_get(tree->left, ref, identify)) ? res
				: ft_tree_get(tree->right, ref, identify));
	}
}
