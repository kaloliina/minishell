/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:11:11 by sojala            #+#    #+#             */
/*   Updated: 2025/05/05 13:53:05 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	invalid_exp_line(char *line, int k, char *replacer)
{
	int	i;

	if (replacer)
		free (replacer);
	i = ft_strlen(line);
	if (k == 1)
		print_error(ERR_AMB, line, NULL);
	return (i);
}

int	expand_line_helper(char *line, char **new_line, t_exp *expand, int i)
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
			i = invalid_exp_line(line, k, replacer);
		free (expand->exp);
	}
	return (i);
}

static char	*handle_quotes_in_line(char *new_line, t_exp *expand)
{
	char	*temp;

	if (!expand->expanded && new_line && *new_line)
	{
		temp = handle_check_quotes(new_line, expand->parser, expand, 0);
		if (temp)
		{
			new_line = temp;
			temp = NULL;
		}
	}
	return (new_line);
}

static char	*expand_line(char *line, t_exp *expand)
{
	int		i;
	int		quote;

	i = 0;
	quote = 0;
	expand->new_line = ft_strdup("");
	if (!expand->new_line)
		fatal_parsing_error(expand->parser, NULL, NULL, ERR_MALLOC);
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && !quote)
			i = expand_line_helper(line, &expand->new_line, expand, i + 1);
		else
		{
			if (line[i] == '\'')
				quote = !quote;
			append_char(&expand->new_line, line[i], expand);
			i++;
		}
	}
	return (handle_quotes_in_line(expand->new_line, expand));
}

char	*handle_filename_helper(char *line, t_data *parser, int status)
{
	t_exp	expand;

	init_exp(&expand, status, parser, NULL);
	if (!ft_strchr(line, '$'))
		expand.new_line = handle_check_quotes(line, parser, NULL, 0);
	else
		expand.new_line = expand_line(line, &expand);
	return (expand.new_line);
}
