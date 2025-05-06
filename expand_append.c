/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_append.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:11:02 by sojala            #+#    #+#             */
/*   Updated: 2025/05/06 10:13:55 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_char_heredoc(char **new_string, char c,
	t_pipes *my_pipes, t_node *heredoc_node)
{
	char	additive[2];
	char	*temp;

	additive[0] = c;
	additive[1] = '\0';
	temp = *new_string;
	*new_string = ft_strjoin(temp, additive);
	free (temp);
	temp = NULL;
	if (!*new_string)
	{
		my_pipes->exit_status = 1;
		fatal_exec_error(ERR_MALLOC, my_pipes, heredoc_node, NULL);
	}
}

void	append_char(char **new_string, char c, t_exp *expand)
{
	char	additive[2];
	char	*temp;

	additive[0] = c;
	additive[1] = '\0';
	temp = *new_string;
	*new_string = ft_strjoin(temp, additive);
	free (temp);
	temp = NULL;
	if (!*new_string)
		fatal_parsing_error(expand->parser, expand, NULL, ERR_MALLOC);
}

void	append_replacer(char **new_string, char *replacer,
	int is_freeable, t_exp *expand)
{
	char	*temp;

	temp = *new_string;
	*new_string = ft_strjoin(temp, replacer);
	free (temp);
	temp = NULL;
	if (!*new_string)
	{
		free (replacer);
		replacer = NULL;
		if (expand->parsing)
			fatal_parsing_error(expand->parser, expand, NULL, ERR_MALLOC);
		else
		{
			free (expand->exp);
			expand->exp = NULL;
			fatal_exec_error(ERR_MALLOC, expand->my_pipes, NULL, NULL);
		}
	}
	if (is_freeable)
	{
		free (replacer);
		replacer = NULL;
	}
}
