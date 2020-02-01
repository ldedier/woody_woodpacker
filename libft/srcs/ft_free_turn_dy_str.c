/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_turn_dy_str.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 15:13:26 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 15:13:31 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_free_turn_dy_str(t_dy_str *dy_str, int ret)
{
	free(dy_str->str);
	free(dy_str);
	return (ret);
}
