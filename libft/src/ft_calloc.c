/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:20:18 by khiidenh          #+#    #+#             */
/*   Updated: 2024/11/13 19:44:53 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Allocates the requested memory and returns a pointer to it. The memory
is set to 0.
- nmemb -> Represents the number of elements to be allocated.
- size -> Represents the size of each element.

- Returns a pointer to allocated memory with all bytes initialized to 0.
- If allocation fails, then it returns a null pointer.
 */
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*memory;

	if (size != 0 && nmemb != 0 && nmemb > SIZE_MAX / size)
		return (NULL);
	memory = malloc(nmemb * size);
	if (memory == NULL)
		return (NULL);
	memory = ft_memset(memory, 0, nmemb * size);
	return (memory);
}
