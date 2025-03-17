/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:02:43 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:46:11 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Is used to copy a block of memory from one location to another.
The function copies the specified number of bytes from one memory location to the
other memory location regardless of the type of data stored.
- dest -> A pointer to the memory location where the copied data will be stored.
- src -> A pointer to the memory location where the data is to be copied from.
- n -> The number of bytes to be copied.

- Memcpy doesn't check for overflow or \0!

- Returns a pointer to the memory location where data is copied.
*/
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = dest;
	s = src;
	while (n-- > 0)
		*d++ = *s++;
	return (dest);
}
