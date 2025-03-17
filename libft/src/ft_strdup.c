/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:23:36 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:47:10 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Duplicates a string. This function returns a pointer to a null-terminated
byte string, which is a duplicate of the string pointed to by s.
- s -> Pointer to the null-terminated byte string to duplicate.

- Returns a pointer to the newly allocated string.
- If error occurred, returns a null pointer.
*/
char	*ft_strdup(const char *s)
{
	size_t	i;
	size_t	length;
	char	*dups;

	if (s == NULL)
		return (NULL);
	i = 0;
	length = ft_strlen(s);
	dups = ft_calloc(length + 1, sizeof(char));
	if (dups == NULL)
		return (NULL);
	while (length > 0)
	{
		dups[i] = s[i];
		i++;
		length--;
	}
	dups[i] = '\0';
	return (dups);
}
