/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_whitespace.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 18:39:18 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/26 23:16:13 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			ft_nb_words_whitespace(char const *s)
{
	int res;
	int parseword;
	int i;

	res = 0;
	parseword = 0;
	i = 0;
	while (s[i])
	{
		if (!ft_isseparator(s[i]) && !parseword)
		{
			res++;
			parseword = 1;
		}
		else if (ft_isseparator(s[i]) && parseword)
			parseword = 0;
		i++;
	}
	return (res);
}

static int	ft_free_split_n(char **split, int n)
{
	int i;

	i = 0;
	while (i < n)
	{
		free(split[i]);
		i++;
	}
	free(split);
	return (1);
}

static int	ft_fill_split(char **res, char const *s)
{
	int		i;
	int		start;
	int		parseword;
	int		word_count;

	i = -1;
	parseword = 0;
	word_count = 0;
	while (s[++i])
	{
		if (!ft_isseparator(s[i]) && !parseword)
		{
			start = i;
			parseword = 1;
		}
		else if (ft_isseparator(s[i]) && parseword)
		{
			if (!(res[word_count++] = ft_strndup(&s[start], i - start)))
				return (ft_free_split_n(res, word_count));
			parseword = 0;
		}
	}
	if (parseword && (!(res[word_count++] = ft_strndup(&s[start], i - start))))
		return (ft_free_split_n(res, word_count));
	return (0);
}

char		**ft_split_whitespace(char const *s)
{
	char	**res;
	int		nb_words;

	nb_words = ft_nb_words_whitespace(s);
	if (!(res = (char **)(malloc(sizeof(char *) * (nb_words + 1)))))
		return (NULL);
	if (ft_fill_split(res, s))
		return (NULL);
	res[nb_words] = 0;
	return (res);
}
