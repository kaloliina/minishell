/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:59:57 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:47:03 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Checks whether the given character is present in the given string or not
- If the character is found, it returns a pointer to it.
- If the character is not found, it returns a null pointer. */
char	*ft_strchr(const char *s, int c)
{
	int				i;
	unsigned char	character;

	character = c;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == character)
			return ((char *)(&s[i]));
		i++;
	}
	if (character == '\0')
		return ((char *)(&s[i]));
	return (NULL);
}
