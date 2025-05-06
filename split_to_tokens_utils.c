/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_to_tokens_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:12:33 by sojala            #+#    #+#             */
/*   Updated: 2025/05/04 18:12:34 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tokensplit_strings(char const *s, int i)
{
	int		strings;
	int		quote;

	strings = 0;
	quote = 0;
	while (s[i])
	{
		if (!is_whitespace(s[i]))
		{
			strings++;
			while (s[i] && (!is_whitespace(s[i]) || quote))
			{
				if ((s[i] == '"' || s[i] == '\'') && !quote)
					quote = s[i];
				else if (quote && s[i] == quote)
					quote = 0;
				i++;
			}
		}
		else
			i++;
	}
	return (strings);
}
