/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 18:38:38 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:45:26 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* To check if the passed character is printable or not.
Returns 1 if true, 0 if false.
*/
int	ft_isprint(int c)
{
	return (c >= 32 && c <= 126);
}
