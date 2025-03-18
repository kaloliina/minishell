/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 09:05:57 by sojala            #+#    #+#             */
/*   Updated: 2024/11/13 09:55:38 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	i = ft_strlen(s);
	if (c == '\0')
		return ((char *) &s[i]);
	while (i > 0)
	{
		if ((unsigned char) s[i - 1] == (unsigned char) c)
		{
			return ((char *) &s[i - 1]);
		}
		i--;
	}
	if (s[i] == c)
		return ((char *) &s[i]);
	return (NULL);
}
