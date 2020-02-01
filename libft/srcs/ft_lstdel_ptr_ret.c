/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel_ptr_ret.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 18:57:59 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/09 18:57:59 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_lstdel_ptr_ret(t_list **list, int ret)
{
	while (*list != NULL)
		ft_lstpop_ptr(list);
	return (ret);
}
