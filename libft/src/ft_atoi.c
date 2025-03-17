/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:16:33 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:44:38 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Accepts a string (which represents an integer) and returns an integer value
- nptr -> The string that is to be converted to an integer.

- Returns an equivalent integer value if input is valid. (Spaces, not alphabetic
characters before numbers, only one minus or plus sign...)
- If the conversion is not valid, the function returns 0.
*/
int	ft_atoi(const char *nptr)
{
	int		i;
	int		check;
	long	result;

	i = 0;
	check = 1;
	result = 0;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == 43 || nptr[i] == 45)
	{
		if (nptr[i] == 45)
			check = -1 * check;
		i++;
	}
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
		if (check == 1 && result < 0)
			return ((int)LONG_MAX);
		if (check == -1 && result < 0)
			return ((int)LONG_MIN);
	}
	return ((int)result * check);
}
