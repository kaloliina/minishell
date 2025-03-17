/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:47:46 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:47:47 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Strlcpy copies 'size - 1 characters from src to dst.
It always null-terminates the result in dst as long as 'size' is
greater than 0.
If the src is longer than 'size - 1', it truncates the copy and
still null-terminates the result.

- It returns the total length of the string it tried to create, which
is the length of 'src'.  */
int	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	length;
	size_t	i;

	i = 0;
	length = ft_strlen(src);
	if (size > 0)
	{
		while (size - 1 > i && length > i)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (length);
}
