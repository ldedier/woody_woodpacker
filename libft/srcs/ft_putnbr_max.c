/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_max.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 16:42:01 by ldedier           #+#    #+#             */
/*   Updated: 2018/05/31 05:07:54 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putnbr_max(u_int64_t nb, t_pf *pf)
{
	if ((u_int64_t)((u_int64_t)nb / (u_int64_t)10) == 0)
		ft_putchar_buff(nb % 10 + '0', pf);
	else
	{
		ft_putnbr_max((u_int64_t)((u_int64_t)nb / (u_int64_t)10), pf);
		ft_putchar_buff(nb % 10 + '0', pf);
	}
}
