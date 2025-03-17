/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 18:37:28 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:45:20 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* To check if the passed character is numeric or not.
- Returns 1 if true, 0 if false.
 */
int	ft_isdigit(int c)
{
	return (c >= 48 && c <= 57);
}
