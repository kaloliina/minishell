/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 12:05:36 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:48:28 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Locates the last occurrence of a character in the given string.
- If character is found, it returns a pointer to it.
- If not found, returns a null pointer.

The terminating null byte is considered part of the string, so that if 'c'
is specified as '\0', these functions return a pointer to the terminator.
 */
char	*ft_strrchr(const char *s, int c)
{
	int		i;
	int		found;
	char	character;

	character = c;
	i = 0;
	found = -1;
	while (s[i] != '\0')
	{
		if (s[i] == character)
			found = i;
		i++;
	}
	if (character == '\0')
		found = i;
	if (found == -1)
		return (NULL);
	else
		return ((char *)(&s[found]));
}
