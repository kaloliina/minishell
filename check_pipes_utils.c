/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipes_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:10:09 by sojala            #+#    #+#             */
/*   Updated: 2025/05/04 18:10:10 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	end_pipe_ctrld(char *temp, t_data *parser, char *line, int backup_fd)
{
	if (!temp)
	{
		if (backup_fd != -1 && close(backup_fd) < 0)
			print_error(ERR_CLOSE, NULL, NULL);
		else
			backup_fd = -1;
		print_error(ERR_EOF, NULL, NULL);
		ft_printf(1, "exit\n");
		fatal_parsing_error(parser, NULL, line, NULL);
	}
}

int	end_pipe_sigint(char *temp, char *line, int *status, int backup_fd)
{
	if (g_signum == SIGINT)
	{
		*status = g_signum + 128;
		g_signum = 0;
		if (dup2(backup_fd, STDIN_FILENO) < 0)
		{
			*status = 1;
			print_error(ERR_FD, NULL, NULL);
		}
		if (backup_fd != -1 && close(backup_fd) < 0)
			print_error(ERR_CLOSE, NULL, NULL);
		else
			backup_fd = -1;
		free (temp);
		free (line);
		return (1);
	}
	return (0);
}

void	end_pipe_helper(char **line, char *temp, char **new_line)
{
	free (*line);
	free (temp);
	*line = *new_line;
	*new_line = NULL;
	temp = NULL;
}
