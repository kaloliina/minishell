/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:10:14 by sojala            #+#    #+#             */
/*   Updated: 2025/05/04 18:10:15 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_invalid_pipes(char *line, int i, int quote)
{
	int	pipes;

	pipes = 0;
	if (line[i] == '|' && !quote)
	{
		i++;
		pipes++;
		while (line[i] == '|' || is_whitespace(line[i]))
		{
			if (line[i] == '|')
				pipes++;
			i++;
		}
		if (line[i] == '\0' || pipes > 1)
			return (1);
	}
	return (0);
}

static int	whitespace_between_pipes(char *line, int i)
{
	if (is_whitespace(line[i]))
	{
		while (is_whitespace(line[i]))
			i++;
		if (line[i] == '|')
			return (1);
	}
	return (0);
}

static char	*check_pipes_helper(char *line, t_data *parser, int i, int j)
{
	char	*new_line;

	if (whitespace_between_pipes(line, i))
	{
		print_error(ERR_SYNTAX, "|", NULL);
		free (line);
		return (NULL);
	}
	new_line = ft_substr(line, 0, (j - 1));
	if (!new_line)
		fatal_parsing_error(parser, NULL, line, ERR_MALLOC);
	free (line);
	line = new_line;
	new_line = NULL;
	return (line);
}

static char	*end_of_line_pipe(char **line, t_data *parser, int *status)
{
	char	*temp;
	char	*new_line;
	int		backup_fd;

	backup_fd = dup(STDIN_FILENO);
	if (backup_fd < 0)
		fatal_parsing_error(parser, NULL, *line, ERR_FD);
	signal(SIGINT, heredoc_signal);
	temp = readline("> ");
	if (end_pipe_sigint(temp, *line, status, backup_fd))
		return (NULL);
	end_pipe_ctrld(temp, parser, *line, backup_fd);
	new_line = ft_strjoin(*line, temp);
	if (!new_line)
	{
		if (backup_fd != -1 && close(backup_fd) < 0)
			print_error(ERR_CLOSE, NULL, NULL);
		fatal_parsing_error(parser, NULL, *line, ERR_MALLOC);
	}
	end_pipe_helper(line, temp, &new_line);
	*line = check_pipes(*line, parser, 0, status);
	if (backup_fd != -1 && close(backup_fd) < 0)
		print_error(ERR_CLOSE, NULL, NULL);
	return (*line);
}

char	*check_pipes(char *line, t_data *parser, int i, int *status)
{
	int		j;
	int		quote;

	quote = 0;
	while (line[i])
	{
		if (is_invalid_pipes(line, i, quote))
		{
			j = i;
			while (line[i] == '|')
				i++;
			if (line[i] != '\0')
				return (check_pipes_helper(line, parser, i, j));
			else
				return (end_of_line_pipe(&line, parser, status));
		}
		else if (!quote && (line[i] == '"' || line[i] == '\''))
			quote = line[i];
		else if (quote && line[i] == quote)
			quote = 0;
		i++;
	}
	return (line);
}
