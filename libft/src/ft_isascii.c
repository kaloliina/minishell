/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 18:31:44 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:45:14 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Checks if the passed character is in ASCII or not. (0-127)
- Returns 1 if true, 0 if false.

- Extended ASCII characters (128-255) are not standard ASCII.
- Unicode characters (such as emoji) are not standard ASCII.
- Negative values are not ASCII.
*/
int	ft_isascii(int c)
{
	return (c >= 0 && c <= 127);
}
