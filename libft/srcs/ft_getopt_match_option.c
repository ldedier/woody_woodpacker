/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 14:52:49 by ldedier           #+#    #+#             */
/*   Updated: 2019/11/04 14:52:49 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strequ_joined(char *to_compare, char *str, char *ing)
{
	while (*str && *to_compare == *str)
	{
		to_compare++;
		str++;
	}
	if (*str)
		return (0);
	while (*to_compare && *ing && *to_compare == *ing)
	{
		to_compare++;
		ing++;
	}
	return (*to_compare == *ing);
}

int		ft_match_arg(t_option *option, char *arg)
{
	char short_buff[2];

	short_buff[0] = option->char_name;
	short_buff[1] = '\0';
	if (option->long_name && !ft_strcmp(arg, option->long_name))
		return (1);
	else if (option->long_name_double_minus && ft_strequ_joined(arg, "--",
			option->long_name_double_minus))
	{
		return (1);
	}
	else if (option->char_name != -1
		&& ft_strequ_joined(arg, "-", short_buff))
	{
		return (1);
	}
	return (0);
}

int		ft_match_position(t_option *option, int index)
{
	return (option->fixed_position == -1 || option->fixed_position == index);
}
