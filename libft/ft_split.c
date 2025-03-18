/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:48:27 by sojala            #+#    #+#             */
/*   Updated: 2024/11/13 17:23:57 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	size_t	ft_start(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] == c && s[i])
		i++;
	return (i);
}

static	size_t	ft_strings(char const *s, char c, size_t i)
{
	size_t	strings;

	strings = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			strings++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		else
			i++;
	}
	return (strings);
}

static	char	**ft_freearray(char **array, size_t k)
{
	size_t	i;

	i = 0;
	while (i <= k)
	{
		free (array[i]);
		i++;
	}
	free (array);
	return (NULL);
}

static	char	**ft_makearray(char const *s, char c, size_t i, char **array)
{
	size_t	k;
	size_t	len;

	k = 0;
	while (s[i] != '\0')
	{
		len = 0;
		while (s[i + len] != c && s[i + len] != '\0')
			len++;
		array[k] = ft_substr(s, i, len);
		if (!array[k])
			return (ft_freearray(array, k));
		i = i + len;
		while (s[i] == c && s[i] != '\0')
			i++;
		k++;
	}
	array[k] = NULL;
	return (array);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	char	**array;

	if (!s)
		return (NULL);
	i = ft_start(s, c);
	array = malloc((ft_strings(s, c, i) + 1) * sizeof(char *));
	if (array == NULL)
		return (NULL);
	return (ft_makearray(s, c, i, array));
}
