/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:06:22 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:46:05 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Compares two blocks of memory.
- s1 -> Pointer to a block of memory.
- s2 -> Pointer to a block of memory.
- n -> This parameter defines the number of bytes to be compared.

- If the return value is <0, then s1 is less than s2.
- If the return value >0, then s2 is less than s1.
- If 0, s1 is equal to s2.
*/
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	unsigned const char	*str1;
	unsigned const char	*str2;

	i = 0;
	str1 = s1;
	str2 = s2;
	while (n > 0)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
		n--;
	}
	return (0);
}
