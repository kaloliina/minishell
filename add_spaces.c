/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_spaces.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:09:21 by sojala            #+#    #+#             */
/*   Updated: 2025/05/06 08:47:55 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_missing_spaces(char *input, int i)
{
	int	extras;

	extras = 0;
	while (input[i])
	{
		if (i > 0 && is_char_redirection(input[i])
			&& !is_whitespace(input[i - 1])
			&& !is_char_redirection(input[i - 1]))
			extras++;
		if (is_char_redirection(input[i]) && input[i + 1]
			&& !is_whitespace(input[i + 1])
			&& !is_char_redirection(input[i + 1]))
			extras++;
		i++;
	}
	return (extras);
}

static char	*add_spaces_helper(char *line, char *input, int i, int j)
{
	int	quote;

	quote = 0;
	while (input[i])
	{
		update_quote(input[i], &quote);
		if (is_missing_pre_space(input, i, quote))
		{
			line[j++] = ' ';
			line[j++] = input[i];
			if (is_missing_post_after_pre_space(input, i))
				line[j++] = ' ';
		}
		else if (is_missing_post_space(input, i, quote))
		{
			line[j++] = input[i];
			line[j++] = ' ';
		}
		else
			line[j++] = input[i];
		i++;
	}
	line[j] = '\0';
	return (line);
}

static void	set_unexpected_token(char *input, int i)
{
	char	token[3];

	token[0] = input[i];
	if (input[i + 1] && (input[i + 1] == input[i]))
	{
		token[1] = input[i + 1];
		token[2] = '\0';
	}
	else
		token[1] = '\0';
	print_error(ERR_SYNTAX, token, NULL);
}

static int	is_invalid_redirections(char *input)
{
	int		i;
	int		quote;

	i = 0;
	quote = 0;
	while (input[i])
	{
		if (!quote && (input[i] == '>' || input[i] == '<'))
		{
			i++;
			if (input[i] && input[i] == input[i - 1])
				i++;
			while (input[i] && is_whitespace(input[i]))
				i++;
			if (input[i] && (input[i] == '>' || input[i] == '<'))
			{
				set_unexpected_token(input, i);
				return (1);
			}
		}
		else
			update_quote(input[i], &quote);
		i++;
	}
	return (0);
}

char	*add_spaces(char *input, t_data *parser)
{
	int		extras;
	char	*line;

	if (is_invalid_redirections(input))
		return (NULL);
	extras = count_missing_spaces(input, 0);
	line = malloc(ft_strlen(input) + extras + 1);
	if (!line)
		fatal_parsing_error(parser, NULL, input, ERR_MALLOC);
	return (add_spaces_helper(line, input, 0, 0));
}
