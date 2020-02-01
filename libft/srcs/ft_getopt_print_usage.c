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

void	print_help(t_opt_parser *parser)
{
	size_t i;

	i = 0;
	while (i < parser->nb_options)
	{
		dprint_option(2, &parser->options[i], 0);
		ft_dprintf(2, ":\t\t%s\n", parser->options[i].description);
		i++;
	}
}

void	process_print_usage(t_opt_parser *parser, size_t i)
{
	size_t	j;
	int		necessity;

	j = 0;
	while (j < parser->nb_options)
	{
		if (parser->options[j].fixed_position == (int)i
				&& (necessity = parser->options[j].necessity) > 0)
		{
			print_necessary_opt(parser, j, i, necessity);
			break ;
		}
		else if (parser->options[j].fixed_position == (int)i)
		{
			print_fixed_opt(parser, j, i);
			break ;
		}
		j++;
	}
	if (j >= parser->nb_options && parser->options[i].fixed_position == -1)
		print_optional_opt(&parser->options[i]);
}

int		print_usage(t_opt_parser *parser)
{
	size_t i;

	ft_dprintf(2, "usage: %s ", parser->progname);
	i = 0;
	while (i < parser->nb_options)
	{
		process_print_usage(parser, i);
		i++;
	}
	ft_dprintf(2, "\n\n");
	print_help(parser);
	return (1);
}
