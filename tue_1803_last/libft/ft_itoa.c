/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:44:52 by sojala            #+#    #+#             */
/*   Updated: 2024/11/13 08:22:50 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	ft_digits(int n)
{
	unsigned int	digits;

	digits = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n = -n;
		digits++;
	}
	while (n > 0)
	{
		n = n / 10;
		digits++;
	}
	return (digits);
}

char	*ft_itoa(int n)
{
	int				nb;
	unsigned int	strlen;
	char			*string;

	nb = n;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	strlen = ft_digits(n);
	string = ft_calloc((strlen + 1), sizeof(char));
	if (string == NULL)
		return (NULL);
	string[strlen] = '\0';
	if (n == 0)
		string[0] = '0';
	if (n < 0)
	{
		string[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		string[--strlen] = (nb % 10) + 48;
		nb = nb / 10;
	}
	return (string);
}
