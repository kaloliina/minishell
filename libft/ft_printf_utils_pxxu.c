/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils_pxxu.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:43:26 by sojala            #+#    #+#             */
/*   Updated: 2025/04/01 14:01:55 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_putptr_hex_fd(unsigned long n, int fd)
{
	int		len;
	int		mod;

	len = 0;
	mod = n % 16;
	if ((n / 16) > 0)
		len += ft_putptr_hex_fd((n / 16), fd);
	if (mod <= 9 && mod >= 0)
	{
		if (ft_putchar_fd((mod + 48), fd) == -1)
			return (-1);
		else
			len++;
	}
	else if (mod <= 15 && mod >= 10)
	{
		if (ft_putchar_fd((mod + 87), fd) == -1)
			return (-1);
		else
			len++;
	}
	return (len);
}

int	ft_putptr_fd(void *ptr, int fd)
{
	int	len;
	int	temp;

	len = 0;
	if (ptr == NULL)
		return (write(fd, "(nil)", 5));
	if (ft_putstr_fd("0x", fd) == -1)
		return (-1);
	else
		len = 2;
	temp = ft_putptr_hex_fd((unsigned long) ptr, fd);
	if (temp == -1)
		return (-1);
	else
		len += temp;
	return (len);
}

int	ft_puthex_lower_fd(unsigned int n, int fd)
{
	int		len;
	int		mod;

	len = 0;
	mod = n % 16;
	if ((n / 16) > 0)
		len += ft_puthex_lower_fd((n / 16), fd);
	if (mod <= 9 && mod >= 0)
	{
		if (ft_putchar_fd((mod + 48), fd) == -1)
			return (-1);
		else
			len++;
	}
	else if (mod <= 15 && mod >= 10)
	{
		if (ft_putchar_fd((mod + 87), fd) == -1)
			return (-1);
		else
			len++;
	}
	return (len);
}

int	ft_puthex_upper_fd(unsigned int n, int fd)
{
	int	len;
	int	mod;

	len = 0;
	mod = n % 16;
	if ((n / 16) > 0)
		len += ft_puthex_upper_fd((n / 16), fd);
	if (mod <= 9 && mod >= 0)
	{
		if (ft_putchar_fd((mod + 48), fd) == -1)
			return (-1);
		else
			len++;
	}
	else if (mod <= 15 && mod >= 10)
	{
		if (ft_putchar_fd((mod + 55), fd) == -1)
			return (-1);
		else
			len++;
	}
	return (len);
}

int	ft_putunsigned_fd(unsigned int n, int fd)
{
	int		len;
	int		i;
	char	string[10];

	i = 0;
	len = 0;
	if (n == 0)
		return (write(fd, "0", 1));
	while (n > 0)
	{
		string[i++] = (n % 10) + '0';
		n = n / 10;
	}
	len = i;
	while (i > 0)
	{
		if (write(fd, &string[--i], 1) == -1)
			return (-1);
	}
	return (len);
}
