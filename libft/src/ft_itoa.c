/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:33:04 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:45:53 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Allocates memory with malloc and returns a string representing the
integer received as an argument.
Negative nubers must be handled.

- n -> The integer to convert.

- Returns the string representing the integer. NULL if the allocation fails.
 */
static size_t	ft_countdigits(int n)
{
	int	counter;

	counter = 0;
	if (n <= 0)
		counter++;
	while (n != 0)
	{
		n = n / 10;
		counter++;
	}
	return (counter);
}

static char	*ft_recursive(int n, size_t counter, char *s)
{
	s[counter] = n % 10 + '0';
	if (n >= 10)
	{
		ft_recursive(n / 10, counter - 1, s);
	}
	return (s);
}

char	*ft_itoa(int n)
{
	size_t	counter;
	char	*number;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	counter = ft_countdigits(n);
	number = ft_calloc(counter + 1, sizeof(char));
	if (number == NULL)
		return (NULL);
	if (n < 0)
	{
		number[0] = '-';
		n = -n;
	}
	number = ft_recursive(n, counter - 1, number);
	number[counter] = '\0';
	return (number);
}
