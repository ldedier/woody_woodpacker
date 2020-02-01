/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 14:52:49 by ldedier           #+#    #+#             */
/*   Updated: 2019/11/27 15:47:40 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	process_option_list_iter(int i, t_opt_parser *parser,
			char *arg, t_opt_update *up)
{
	size_t	j;
	int		ret;

	j = 0;
	while (j < parser->nb_options)
	{
		if (parser->options[j].char_name == arg[i])
		{
			if (parser->options[j].argument != NULL && arg[i + 1])
			{
				ft_dprintf(2, "option -%c requires an argument\n", arg[i]);
				return (E_OPT_RET_ERROR_NO_ARGUMENT);
			}
			if ((ret = parser->options[j].update(up->argv, up->env)))
				return (ret);
			break ;
		}
		j++;
	}
	if (j >= parser->nb_options)
	{
		ft_dprintf(2, "-%c: unkown option\n", arg[i]);
		return (E_OPT_RET_ERROR_UNKNOWN_OPTION);
	}
	return (E_OPT_RET_ERROR_UNKNOWN_OPTION_ITER);
}

static int	process_option_list(t_opt_parser *parser, char ***argv, void *env)
{
	char			*arg;
	size_t			i;
	int				ret;
	t_opt_update	up;

	arg = **argv;
	up.env = env;
	up.argv = argv;
	i = 1;
	while (arg[i])
	{
		if ((ret = process_option_list_iter(i, parser, arg, &up))
			!= E_OPT_RET_ERROR_UNKNOWN_OPTION_ITER)
			return (ret);
		i++;
	}
	return (0);
}

int			await_necessary_options(t_opt_parser *parser, int index)
{
	size_t i;

	i = 0;
	while (i < parser->nb_options)
	{
		if (parser->options[i].necessity > 0
			&& parser->options[i].fixed_position != -1
			&& parser->options[i].fixed_position > index)
		{
			return (1);
		}
		i++;
	}
	return (0);
}

static int	process_arg_iter(t_opt_parser *parser, size_t i,
	int index, t_opt_update *up)
{
	size_t	j;
	int		necessity;

	if (parser->options[i].fixed_position == index
			&& (necessity = parser->options[i].necessity) > 0)
	{
		j = i;
		while (j < parser->nb_options)
		{
			if (ft_match_position(&parser->options[j], index)
					&& parser->options[j].necessity == necessity
					&& ft_match_arg(&parser->options[j], **up->argv))
			{
				return (parser->options[j].update(up->argv, up->env));
			}
			j++;
		}
		if (j == parser->nb_options)
			return (print_necessaries_options(parser, index));
	}
	else if (ft_match_position(&parser->options[i], index)
			&& ft_match_arg(&parser->options[i], **up->argv))
		return (parser->options[i].update(up->argv, up->env));
	return (E_OPT_RET_ERROR_UNKNOWN_OPTION_ITER);
}

int			process_arg(int index, char ***argv,
				t_opt_parser *parser, void *env)
{
	size_t			i;
	int				ret;
	t_opt_update	up;

	up.env = env;
	up.argv = argv;
	i = 0;
	while (i < parser->nb_options)
	{
		if ((ret = process_arg_iter(parser, i, index, &up))
			!= E_OPT_RET_ERROR_UNKNOWN_OPTION_ITER)
			return (ret);
		i++;
	}
	if (describe_option(**argv))
		return (process_option_list(parser, argv, env));
	if (await_necessary_options(parser, index))
	{
		ft_dprintf(2, "%s: unknown option\n", **argv);
		return (E_OPT_RET_ERROR_UNKNOWN_OPTION);
	}
	return (E_OPT_RET_END_OF_PARSING);
}
