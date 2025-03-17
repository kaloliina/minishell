/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:33:43 by khiidenh          #+#    #+#             */
/*   Updated: 2025/01/22 18:27:21 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Allocates memory with malloc and returns an array of strings obtained by
splitting 's' using the character 'c' as a delimiter. The array must end with
a NULL pointer.

- s -> The string to be split.
- c  -> The delimiter character.

- Returns the array of new strings resulting from the split. NULL if the
allocation fails. */
static size_t	ft_arraylength(char const *s, char c)
{
	size_t	i;
	size_t	length;

	i = 0;
	length = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			length++;
		i++;
	}
	return (length);
}

static char	**ft_freemem(char **s, size_t j)
{
	if (s[j] == NULL)
	{
		while (j > 0)
		{
			free (s[j]);
			j--;
		}
		free (s);
		return (NULL);
	}
	return (s);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	start;
	char	**sarray;

	i = 0;
	j = 0;
	sarray = ft_calloc((ft_arraylength(s, c) + 1), sizeof(char *));
	if (sarray == NULL)
		return (NULL);
	sarray[ft_arraylength(s, c)] = NULL;
	while (ft_arraylength(s, c) > j)
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		start = i;
		while (s[i] != c && s[i] != '\0')
			i++;
		sarray[j] = ft_substr(s, start, i - start);
		if (sarray[j] == NULL)
			return (ft_freemem(sarray, j));
		j++;
	}
	return (sarray);
}
