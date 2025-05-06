/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:59:23 by sojala            #+#    #+#             */
/*   Updated: 2025/05/06 17:59:24 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	hd_invalid_exp_line(char *line, char *replacer)
{
	int	i;

	if (replacer)
	{
		free (replacer);
		replacer = NULL;
	}
	i = ft_strlen(line);
	return (i);
}

int	hd_expand_line_helper(char *line, char **new_line, t_exp *expand, int i)
{
	int		k;
	char	*replacer;

	expand->exp = find_exp(line, &i, &k, expand);
	if ((expand->exp && *expand->exp) || (line[i] == '?' && line[i - 1] == '$'))
	{
		replacer = find_replacer(line, i, expand);
		if (replacer && *replacer)
		{
			if (!is_quote(line))
				expand->expanded = 1;
			append_replacer(new_line, replacer, 1, expand);
			if (expand->exp && *expand->exp)
				i = k + ft_strlen(expand->exp);
			else
				i = k + 1;
		}
		else if (line[i] && line[i + 1])
			i = k + ft_strlen(expand->exp);
		else
			i = hd_invalid_exp_line(line, replacer);
		free (expand->exp);
		expand->exp = NULL;
	}
	return (i);
}
