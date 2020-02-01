/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 14:52:49 by ldedier           #+#    #+#             */
/*   Updated: 2019/11/27 15:43:01 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_getopt_ret(t_opt_parser *parser, int parsed_options)
{
	if (parser->need_options
		&& await_necessary_options(parser, parsed_options - 1))
	{
		print_usage(parser);
		return (E_OPT_RET_ERROR_NO_OPTIONS);
	}
	return (0);
}

int			ft_getopt(char ***argv, t_opt_parser *parser, void *env)
{
	char	**argv_save;
	int		ret;
	int		parsed_options;

	parsed_options = 0;
	argv_save = *argv;
	while (**argv)
	{
		if (!ft_strcmp("--", **argv))
		{
			parsed_options = *argv - argv_save;
			(*argv)++;
			break ;
		}
		if ((ret = process_arg(*argv - argv_save, argv, parser, env)))
		{
			if (ret == E_OPT_RET_ERROR_UNKNOWN_OPTION
				|| ret == E_OPT_RET_ERROR_NO_ARGUMENT)
				print_usage(parser);
			return (ret == E_OPT_RET_END_OF_PARSING ? 0 : ret);
		}
		(*argv)++;
		parsed_options = *argv - argv_save;
	}
	return (ft_getopt_ret(parser, parsed_options));
}
