/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 15:43:06 by ldedier           #+#    #+#             */
/*   Updated: 2019/11/27 15:44:59 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	init_opt_parser(t_opt_parser *parser, t_option *opts,
	size_t size, int need_opts)
{
	parser->options = opts;
	parser->nb_options = size;
	parser->need_options = need_opts;
}

void	print_number(int fd, int number)
{
	int mod;

	mod = number % 10;
	if (mod == 1)
		ft_dprintf(fd, "%dst", number);
	else if (mod == 2)
		ft_dprintf(fd, "%dnd", number);
	else
		ft_dprintf(fd, "%dth", number);
}

int		print_necessaries_options(t_opt_parser *parser, int index)
{
	size_t	i;
	size_t	j;

	print_number(2, index + 1);
	ft_dprintf(2, " argument has to be one of those:\n");
	i = 0;
	j = 0;
	while (i < parser->nb_options)
	{
		if (parser->options[i].fixed_position == index)
		{
			if (j++ == 0)
				ft_dprintf(2, "\n");
			dprint_option(2, &parser->options[i], 1);
			ft_dprintf(2, "\n");
		}
		i++;
	}
	ft_dprintf(2, "\n");
	return (E_OPT_RET_ERROR_UNKNOWN_OPTION);
}

int		describe_option(char *arg)
{
	return (ft_strlen(arg) >= 2 && arg[0] == '-');
}

int		ft_need_option(t_opt_parser *parser)
{
	size_t i;

	i = 0;
	while (i < parser->nb_options)
	{
		if (parser->options[i++].necessity > 0)
			return (1);
	}
	return (0);
}
