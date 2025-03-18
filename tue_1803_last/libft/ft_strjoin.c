/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 10:29:55 by sojala            #+#    #+#             */
/*   Updated: 2024/11/13 09:34:35 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*string;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	string = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (string == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] != '\0')
		string[j++] = s1[i++];
	i = 0;
	while (s2[i] != '\0')
		string[j++] = s2[i++];
	string[j] = '\0';
	return (string);
}
