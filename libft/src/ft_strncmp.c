/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 14:55:30 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 16:37:15 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Compares the first n characters of two strings and returns an integer
indicating which one is greater.
- s1 -> First string to be compared.
- s2 -> Second string to be compared.
- n -> Maximum number of characters to be compared.

- Returns negative value if s1 is less than s2.
- Returns positive value if s2 is less than s1.
- Returns 0 if s1 is equal to s2.
*/
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] != '\0' && n > 0) || (s2[i] != '\0' && n > 0))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
		n--;
	}
	return (0);
}
