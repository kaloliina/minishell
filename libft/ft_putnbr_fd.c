/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 16:50:19 by sojala            #+#    #+#             */
/*   Updated: 2024/11/13 17:19:57 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	int	div;
	int	mod;

	div = n / 10;
	mod = n % 10;
	if (n == -2147483648)
		ft_putstr_fd("-2147483648", fd);
	else if (n < 0)
	{
		ft_putchar_fd('-', fd);
		ft_putnbr_fd(-n, fd);
	}
	else if (n <= 9 && n >= 0)
		ft_putchar_fd((n + 48), fd);
	else if (div > 9)
	{
		ft_putnbr_fd(div, fd);
		ft_putchar_fd((mod + 48), fd);
	}
	else
	{
		ft_putchar_fd((div + 48), fd);
		ft_putchar_fd((mod + 48), fd);
	}
}
