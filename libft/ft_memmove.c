/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 08:13:36 by sojala            #+#    #+#             */
/*   Updated: 2024/11/13 08:42:25 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*dest_a;
	const unsigned char	*src_a;

	dest_a = (unsigned char *)dest;
	src_a = (const unsigned char *)src;
	i = 0;
	if (!dest && !src)
		return (dest);
	i = 0;
	if (dest_a <= src_a)
		return (ft_memcpy(dest, src, n));
	else if (dest_a > src_a)
	{
		i = n - 1;
		while (n > 0)
		{
			dest_a[i] = src_a[i];
			n--;
			i--;
		}
	}
	return (dest);
}
