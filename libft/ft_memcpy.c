/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:53:36 by sojala            #+#    #+#             */
/*   Updated: 2024/11/13 08:34:26 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*dest_a;
	const unsigned char	*src_a;
	const unsigned char	*end;

	if (!dest && !src)
		return (dest);
	dest_a = (unsigned char *)dest;
	src_a = (const unsigned char *)src;
	end = src_a + n;
	while (src_a < end)
		*dest_a++ = *src_a++;
	return (dest);
}
