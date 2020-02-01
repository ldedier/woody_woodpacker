/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt_print_option.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 15:03:02 by ldedier           #+#    #+#             */
/*   Updated: 2019/11/27 14:46:08 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		dprint_option(int fd, t_option *option, int print_argument)
{
	if (option->char_name != -1)
		ft_dprintf(fd, "-%c", option->char_name);
	else if (option->long_name_double_minus)
		ft_dprintf(fd, "--%s", option->long_name_double_minus);
	else if (option->long_name)
		ft_dprintf(fd, "%s", option->long_name);
	else
		ft_dprintf(fd, "%s", "**unprintable option**");
	if (option->argument && print_argument)
		ft_dprintf(fd, " %s", option->argument);
	return (1);
}

void	print_necessary_opt(t_opt_parser *parser, int j, int i, int necessity)
{
	size_t	k;
	int		first;

	first = 1;
	k = j;
	while (k < parser->nb_options)
	{
		if (parser->options[k].fixed_position != -1
				&& parser->options[k].fixed_position == i
				&& parser->options[k].necessity == necessity)
		{
			if (first == 1)
				ft_dprintf(2, " { ");
			else
				ft_dprintf(2, " , ");
			dprint_option(2, &parser->options[k], 1);
			first = 0;
		}
		k++;
	}
	if (first == 0)
		ft_dprintf(2, " }");
}

void	print_fixed_opt(t_opt_parser *parser, int j, int i)
{
	size_t	k;
	int		first;

	first = 1;
	k = j;
	while (k < parser->nb_options)
	{
		if (parser->options[k].fixed_position != -1
				&& parser->options[k].fixed_position == i)
		{
			if (first == 1)
				ft_dprintf(2, " [ ");
			else
				ft_dprintf(2, " , ");
			dprint_option(2, &parser->options[k], 1);
			first = 0;
		}
		k++;
	}
	if (first == 0)
		ft_dprintf(2, " ]");
}

void	print_optional_opt(t_option *option)
{
	ft_dprintf(2, " [");
	dprint_option(2, option, 1);
	ft_dprintf(2, "]");
}
