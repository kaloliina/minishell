/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:12:45 by sojala            #+#    #+#             */
/*   Updated: 2025/05/04 18:12:46 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_elements(char **sections)
{
	int	i;

	i = 0;
	while (sections[i])
		i++;
	return (i);
}

int	is_quote(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

int	is_only_quotes(char *s)
{
	if (((s[0] == '\'' && s[1] == '\'')
			|| (s[0] == '"' && s[1] == '"'))
		&& s[2] == '\0')
		return (1);
	return (0);
}

int	is_exp_delimiter(char c)
{
	return (!(ft_isalnum(c) || c == '_'));
}

int	is_char_redirection(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}
