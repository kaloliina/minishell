/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 10:04:33 by sojala            #+#    #+#             */
/*   Updated: 2024/11/13 10:02:18 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	int		dstlen;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		dstlen = 0;
	else
	{
		if (ft_strlen(s) - start < len)
			dstlen = ft_strlen(s) - start;
		else
			dstlen = len;
	}
	substr = malloc((sizeof(char) * dstlen) + 1);
	if (substr == NULL)
		return (NULL);
	ft_memcpy(substr, s + start, dstlen);
	substr[dstlen] = '\0';
	return (substr);
}
