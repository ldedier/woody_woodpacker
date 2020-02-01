/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_delete_node_ptr.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 15:21:30 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/20 17:29:47 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_delete_node_ptr(t_list **prev, t_list **ptr, t_list **list)
{
	if (*prev == NULL)
		*list = (*ptr)->next;
	else
		(*prev)->next = (*ptr)->next;
	free(*ptr);
	return (0);
}
