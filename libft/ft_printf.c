/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 14:01:22 by sojala            #+#    #+#             */
/*   Updated: 2025/04/01 13:53:48 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_format(int fd, const char *string, size_t i, va_list args)
{
	if (string[i] == 'c')
		return (ft_putchar_fd((char) va_arg(args, int), fd));
	else if (string[i] == 'd' || string[i] == 'i')
		return (ft_putnbr_fd(va_arg(args, int), fd));
	else if (string[i] == 's')
		return (ft_putstr_fd(va_arg(args, char *), fd));
	else if (string[i] == 'p')
		return (ft_putptr_fd(va_arg(args, void *), fd));
	else if (string[i] == 'u')
		return (ft_putunsigned_fd(va_arg(args, unsigned int), fd));
	else if (string[i] == 'x')
		return (ft_puthex_lower_fd(va_arg(args, unsigned int), fd));
	else if (string[i] == 'X')
		return (ft_puthex_upper_fd(va_arg(args, unsigned int), fd));
	else if (string[i] == '\0')
		return (-1);
	else
		return (ft_putchar_fd('%', fd));
}

int	ft_printf(int fd, const char *string, ...)
{
	va_list	args;
	int		i;
	int		length;
	int		temp;

	i = 0;
	length = 0;
	if (!string)
		return (-1);
	va_start(args, string);
	while (string[i] && length != -1)
	{
		if (string[i] == '%')
			temp = ft_format(fd, string, ++i, args);
		else
			temp = ft_putchar_fd(string[i], fd);
		if (temp == -1)
			length = temp;
		else
			length += temp;
		i++;
	}
	va_end(args);
	return (length);
}
