/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_spaces_utils_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:08:59 by sojala            #+#    #+#             */
/*   Updated: 2025/05/05 17:41:51 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	is_invalid_redirections(char *input)
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
