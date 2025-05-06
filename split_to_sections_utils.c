/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_to_sections_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:12:25 by sojala            #+#    #+#             */
/*   Updated: 2025/05/06 10:15:50 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**split_freearray(char **array, int j, int *error)
{
	int	i;

	i = 0;
	while (i <= j)
		free (array[i++]);
	free (array);
	array = NULL;
	*error = 1;
	return (NULL);
}

int	split_checkquote(char const *s, int i, char quote)
{
	while (s[i])
	{
		if (s[i] == quote)
			return (1);
		i++;
	}
	return (0);
}

int	sectionsplit_strings(char const *s, char c, int i)
{
	int		strings;
	int		quote;

	strings = 0;
	quote = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			strings++;
			while (s[i] && (s[i] != c || quote))
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
