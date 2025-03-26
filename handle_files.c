/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_files.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 17:44:40 by khiidenh          #+#    #+#             */
/*   Updated: 2025/03/26 11:24:38 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//remember error cases.. :)
int	open_infile(char *file)
{
	int fd = open(file, O_RDONLY);
	return (fd);
}

int	set_outfile(char *file, int append)
{
	int fd;

	if (append == 0)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (append == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}
