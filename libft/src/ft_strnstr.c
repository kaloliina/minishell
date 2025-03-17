/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:11:30 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:48:21 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Locates the first occurrence of the string little in string big, where not
more than len characters are searched.
- big -> This is the string from where the occurrence of little is searched.
- little -> This is the string that is searched in 'big'.
- len -> The amount of characters to be searched in 'big'.

- If little is an empty string, big is returned.
- If little occurs nowhere in big, NULL is returned.
- If little is found in big, a pointer to the first character of the first
occurrence of little is returned.
 */
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (little[j] == '\0')
		return ((char *)(big));
	while (len > i && big[i] != '\0')
	{
		j = 0;
		while (big[i + j] == little[j] && i + j < len)
		{
			j++;
			if (little[j] == '\0')
				return ((char *)(&big[i]));
		}
		i++;
	}
	return (NULL);
}
