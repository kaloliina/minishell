/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:21:33 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:47:53 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Takes a string as an argument and returns its length. */
size_t	ft_strlen(const char *s)
{
	size_t	length;

	length = 0;
	while (*s++ != '\0')
		length ++;
	return (length);
}
