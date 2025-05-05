/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:11:07 by sojala            #+#    #+#             */
/*   Updated: 2025/05/05 18:28:49 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_cmd_helper(char *arg, t_exp *expand, int new_arg, int i)
{
	int		k;
	char	*replacer;

	expand->exp = find_exp(arg, &i, &k, expand);
	if ((expand->exp && *expand->exp) || (arg[i] == '?' && arg[i - 1] == '$'))
	{
		replacer = find_replacer(arg, i, expand);
		if (replacer)
		{
			if (!is_quote(arg))
				expand->expanded = 1;
			append_replacer(&(expand->new_cmd[new_arg]), replacer, 1, expand);
			if (expand->exp && *expand->exp)
				i = k + ft_strlen(expand->exp);
			else
				i = k + 1;
		}
		else if (arg[i] && arg[i + 1])
			i = k + ft_strlen(expand->exp);
		else if (k == 1)
			expand->no_element = 1;
		free (expand->exp);
		expand->exp = NULL;
	}
	return (i);
}

static int	is_only_dollar(char *arg, int i)
{
	if (arg[i + 1] == '$' && ((arg[i] == '"' && arg[i + 2] == '"')
			|| (arg[i] == '\'' && arg[i + 2] == '\'')))
		return (1);
	return (0);
}

static int	is_expandable(char *arg, int i, int quote)
{
	if (arg[i] == '$' && arg[i + 1]
		&& !quote && !is_whitespace(arg[i + 1])
		&& (!is_exp_delimiter(arg[i + 1]) || arg[i + 1] == '?'))
		return (1);
	return (0);
}

static void	expand_cmd(char **cmd, t_exp *expand, int *arg, int *new_arg)
{
	int		i;
	int		s_quote;
	int		d_quote;

	i = 0;
	s_quote = 0;
	d_quote = 0;
	init_new_arg(expand, new_arg);
	while (cmd[*arg][i])
	{
		if (is_only_dollar(cmd[*arg], i))
		{
			append_char(&expand->new_cmd[*new_arg], '$', expand);
			i += 3;
		}
		else if (is_expandable(cmd[*arg], i, s_quote))
			i = expand_cmd_helper(cmd[*arg], expand, *new_arg, i + 1);
		else
		{
			update_single_quote(cmd[*arg][i], &s_quote, &d_quote);
			append_char(&expand->new_cmd[*new_arg], cmd[*arg][i], expand);
			i++;
		}
	}
	handle_quotes_in_expansion(expand, new_arg, arg);
}

char	**handle_cmd_helper(char **cmd, t_data *parser, int status, int arg)
{
	int		new_arg;
	t_exp	expand;

	new_arg = 0;
	init_exp(&expand, status, parser, NULL);
	init_new_cmd(cmd, parser, &expand);
	while (cmd[arg])
	{
		if (!ft_strchr(cmd[arg], '$'))
		{
			expand.new_cmd[new_arg]
				= handle_check_quotes(cmd[arg++], parser, &expand, new_arg);
			new_arg++;
		}
		else
			expand_cmd(cmd, &expand, &arg, &new_arg);
	}
	if (!new_arg)
	{
		expand.new_cmd[new_arg++] = ft_strdup("");
		if (!expand.new_cmd[new_arg - 1])
			fatal_parsing_error(parser, &expand, NULL, ERR_MALLOC);
	}
	expand.new_cmd[new_arg] = NULL;
	return (expand.new_cmd);
}
