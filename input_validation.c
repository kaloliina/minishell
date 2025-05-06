/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 08:47:25 by sojala            #+#    #+#             */
/*   Updated: 2025/05/06 08:47:27 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_closing_quote(char *s, int i, int quote)
{
	while (s[i])
	{
		if (s[i] == quote)
			return (i + 1);
		i++;
	}
	return (0);
}

int	is_unclosed_quote(char *input)
{
	int	i;
	int	temp;

	i = 0;
	temp = 0;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			temp = check_closing_quote(input, i + 1, input[i]);
			if (!temp)
			{
				print_error("syntax error: unclosed quotes\n", NULL, NULL);
				return (-1);
			}
			else
				i = temp;
		}
		else
			i++;
	}
	return (0);
}

int	is_only_pipes(char *input)
{
	int	i;

	i = 0;
	while (is_whitespace(input[i]))
		i++;
	if (input[i] != '|')
		return (0);
	if (input[i] == '|' && input[i + 1] == '|')
		print_error(ERR_SYNTAX, "||", NULL);
	else
		print_error(ERR_SYNTAX, "|", NULL);
	return (1);
}
