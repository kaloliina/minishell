/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:39:01 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:46:23 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Is used to fill a block of memory with a particular value.
- s -> Starting address of the memory to be filled.
- c -> Value to fill the block of memory with. Is converted to an unsigned char.
- n -> Number of bytes to be filled.

- Returns a pointer to the memory area s.

- !! It's the caller's responsibility to ensure that 'n' does not exceed the
allocated memory size. If n is larger, it will lead to undefined behaviour.
*/
void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	character;

	character = c;
	str = s;
	while (n-- > 0)
	{
		*str = character;
		str++;
	}
	return (s);
}
