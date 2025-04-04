/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils_csdi.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sonjaojala <sonjaojala@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:53:00 by sojala            #+#    #+#             */
/*   Updated: 2025/04/04 08:36:06 by sonjaojala       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_putnbr_write(int n, int neg, int fd)
{
	int		i;
	int		len;
	char	string[11];

	i = 0;
	while (n > 0)
	{
		string[i++] = (n % 10) + '0';
		n = n / 10;
	}
	if (neg == 1)
		string[i++] = '-';
	len = i;
	while (i > 0)
	{
		if (write(fd, &string[--i], 1) == -1)
			return (-1);
	}
	return (len);
}

int	ft_putnbr_fd(int n, int fd)
{
	int		neg;

	neg = 0;
	if (n == -2147483648)
		return (ft_putstr_fd("-2147483648", fd));
	if (n == 0)
		return (write(fd, "0", 1));
	if (n < 0)
	{
		neg = 1;
		n = -n;
	}
	return (ft_putnbr_write(n, neg, fd));
}
