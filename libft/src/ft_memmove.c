/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:24:01 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:46:17 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Is used to copy a block of memory from one location to another.
The memory areas may overlap: copying takes place as though the bytes in src
are first copied into a temp array that does not overlap src or dest, and the
bytes are then copied to dest.
- dest -> A pointer to the memory location where the copied data will
be stored.
- src -> A pointer to the memory location where the data is to be copied from.
- n -> The number of bytes to be copied.

- Returns a pointer to the memory location where data is copied. */
void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	d = dest;
	s = src;
	i = 0;
	if (dest <= src)
	{
		while (n-- > 0)
		{
			d[i] = s[i];
			i++;
		}
	}
	else if (dest > src)
	{
		i = n - 1;
		while (n-- > 0)
		{
			d[i] = s[i];
			i--;
		}
	}
	return (dest);
}
