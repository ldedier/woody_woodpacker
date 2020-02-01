/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 15:04:18 by ldedier           #+#    #+#             */
/*   Updated: 2019/11/27 15:47:04 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GETOPT_H
# define FT_GETOPT_H

typedef enum		e_opt_ret
{
	E_OPT_RET_OK,
	E_OPT_RET_NOT_AN_OPTION,
	E_OPT_RET_ERROR_FATAL,
	E_OPT_RET_ERROR_SEMANTIC,
	E_OPT_RET_ERROR_UNKNOWN_OPTION,
	E_OPT_RET_ERROR_UNKNOWN_OPTION_ITER,
	E_OPT_RET_ERROR_NO_ARGUMENT,
	E_OPT_RET_ERROR_NO_OPTIONS,
	E_OPT_RET_END_OF_PARSING,
}					t_opt_ret;

typedef struct		s_option
{
	char			char_name;
	char			*long_name_double_minus;
	char			*long_name;
	char			*argument;
	t_opt_ret		(*update)(char ***arg, void *env);
	char			*description;
	int				fixed_position;
	int				necessity;
}					t_option;

typedef struct		s_opt_update
{
	char			***argv;
	void			*env;
}					t_opt_update;

typedef struct		s_opt_parser
{
	char			*progname;
	t_option		*options;
	size_t			nb_options;
	int				need_options : 1;
}					t_opt_parser;

void				init_opt_parser(t_opt_parser *parser,
						t_option *opts, size_t size, int need_options);
int					ft_getopt(char ***argv, t_opt_parser *parser, void *env);
int					print_usage(t_opt_parser *parser);

int					dprint_option(int fd, t_option *option, int print_argument);
void				print_fixed_opt(t_opt_parser *parser, int j, int i);
void				print_necessary_opt(t_opt_parser *parser,
						int j, int i, int necessity);
void				print_optional_opt(t_option *option);

int					ft_strequ_joined(char *to_compare, char *str, char *ing);
int					ft_match_arg(t_option *option, char *arg);
int					ft_match_position(t_option *option, int index);
int					ft_need_option(t_opt_parser *parser);
int					describe_option(char *arg);
int					process_arg(int index, char ***argv,
						t_opt_parser *parser, void *env);
int					await_necessary_options(t_opt_parser *parser, int index);
int					print_necessaries_options(t_opt_parser *parser, int index);
#endif
