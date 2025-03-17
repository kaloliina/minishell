/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:58:22 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:46:43 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Outputs the integer 'n' to the given file descriptor.
- n -> The integer to output.
- fd -> The file descriptor on which to write.
 */
void	ft_putnbr_fd(int n, int fd)
{
	char	number;

	if (n < 0)
	{
		if (n == -2147483648)
		{
			write(fd, "-2147483648", 11);
			return ;
		}
		else
		{
			write(fd, "-", 1);
			n = -n;
		}
	}
	if (n >= 10)
	{
		ft_putnbr_fd(n / 10, fd);
	}
	number = (n % 10) + '0';
	write(fd, &number, 1);
}
