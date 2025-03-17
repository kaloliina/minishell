/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:05:46 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:48:06 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Applies the function 'f' to each character of the string 's', and passing
it's index as first argument to create a new string (with malloc) resulting
from successive applications of 'f'.
- s -> The string on which to iterate.
- f -> The funtion to apply to each character.

Returns the string created from the successive applications of 'f' or
NULL if the allocation fails. */
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	int		j;
	char	*iteration;

	iteration = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (iteration == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		iteration[j] = f(i, s[i]);
		i++;
		j++;
	}
	iteration[j] = '\0';
	return (iteration);
}
