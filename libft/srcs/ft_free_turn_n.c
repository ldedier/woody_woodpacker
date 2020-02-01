/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_turn_n.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/24 21:23:17 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/26 23:14:51 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_free_turn_2(void *to_free, void *to_free2, int ret)
{
	free(to_free);
	free(to_free2);
	return (ret);
}

int		ft_free_turn_3(void *to_free, void *to_free2, void *to_free3, int ret)
{
	free(to_free);
	free(to_free2);
	free(to_free3);
	return (ret);
}
