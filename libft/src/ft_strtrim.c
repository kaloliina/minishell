/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 11:51:57 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:48:33 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Allocates memory with malloc and returns a copy of 's1' with the characters
specified in 'set' removed from the beginning and the end of the string.
- s1 -> The string to be trimmed.
- set -> The referene set of characters to trim.

- Returns the trimmed string or NULL if the allocation fails.
*/
static size_t	ft_edgecheck(char const *s1, char const *set, size_t endorstart)
{
	size_t	i;
	size_t	j;
	size_t	found;
	size_t	currentchar;

	i = 0;
	j = 0;
	found = 0;
	while (s1[i] != '\0' && j < ft_strlen(set))
	{
		if (endorstart == 0)
			currentchar = i;
		if (endorstart == 1)
			currentchar = ft_strlen(s1) - i - 1;
		if (s1[currentchar] == set[j])
		{
			found++;
			j = 0;
			i++;
		}
		else
			j++;
	}
	return (found);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	found_start;
	size_t	found_end;
	char	*new;

	found_start = ft_edgecheck(s1, set, 0);
	found_end = ft_edgecheck(s1, set, 1);
	if (found_start == ft_strlen(s1) || found_end == ft_strlen(s1))
		return (new = ft_calloc(1, 1));
	new = ft_substr(s1, found_start, ft_strlen(s1) - found_start - found_end);
	if (new == NULL)
		return (NULL);
	return (new);
}
