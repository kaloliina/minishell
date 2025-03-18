/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:49:35 by sojala            #+#    #+#             */
/*   Updated: 2024/11/13 17:23:18 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	size_t	ft_start(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (set[j] != '\0')
	{
		if (s1[i] == set[j])
		{
			j = 0;
			i++;
		}
		else
			j++;
	}
	return (i);
}

static	size_t	ft_end(char const *s1, char const *set, size_t i)
{
	size_t	j;
	size_t	k;

	j = 0;
	k = ft_strlen(s1) - 1;
	while (k > i && set[j] != '\0')
	{
		if (s1[k] == set[j])
		{
			j = 0;
			k--;
		}
		else
			j++;
	}
	return (k);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*string;
	size_t	i;
	size_t	k;
	size_t	l;

	if (!s1 || !set)
		return (NULL);
	if (!*s1)
		return (ft_calloc(1, 1));
	i = ft_start(s1, set);
	k = ft_end(s1, set, i);
	string = malloc((k - i) + 2);
	if (string == NULL)
		return (NULL);
	l = 0;
	while (i <= k)
	{
		string[l] = s1[i];
		i++;
		l++;
	}
	string[l] = '\0';
	return (string);
}
