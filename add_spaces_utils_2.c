/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_spaces_utils_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:08:59 by sojala            #+#    #+#             */
/*   Updated: 2025/05/05 13:23:09 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_unexpected_token(char *token, char *input, int i)
{
	token[0] = input[i];
	if (input[i + 1] && (input[i + 1] == input[i]))
	{
		token[1] = input[i + 1];
		token[2] = '\0';
	}
	else
		token[1] = '\0';
}

int	is_invalid_redirections(char *input)
{
	int		i;
	char	token[3];

	i = 0;
	while (input[i])
	{
		if (input[i] == '>' || input[i] == '<')
		{
			i++;
			if (input[i] && input[i] == input[i - 1])
				i++;
			while (input[i] && is_whitespace(input[i]))
				i++;
			if (input[i] && (input[i] == '>' || input[i] == '<'))
			{
				set_unexpected_token(token, input, i);
				print_error(ERR_SYNTAX, token, NULL);
				return (1);
			}
		}
		i++;
	}
	return (0);
}
