/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:00:23 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:45:59 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Searches the first occurrence of the character c in the first n bytes
of the string.

- s -> Pointer to the memory block where the search is performed.
- c -> Value to be passed as an int, but the function performs a byte per
byte search using the unsigned char conversion of this value.
- n -> Number of bytes to be analyzed.

- Function returns a pointer to the matching byte or NULL if the character
does not occur in the given memory area.
*/
void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char		*str;
	unsigned char			character;
	size_t					i;

	character = c;
	str = s;
	i = 0;
	while (n > 0)
	{
		if (str[i] == character)
			return ((void *)(&str[i]));
		i++;
		n--;
	}
	return (NULL);
}
