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

static int	spaces_between_pipes(char *line, int i)
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

	if (spaces_between_pipes(line, i))
	{
		ft_printf(2, SYNTAX, "|");
		return (NULL);
	}
	new_line = ft_substr(line, 0, (j - 1));
	if (!new_line)
		fatal_parsing_exit(parser, NULL, line, MALLOC);
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
	signal(SIGINT, heredoc_signal);
	temp = readline("> ");
	if (end_pipe_sigint(backup_fd, temp, *line, status))
		return (NULL);
	check_for_ctrld(temp, parser, *line, backup_fd);
	new_line = ft_strjoin(*line, temp);
	if (!new_line)
	{
		close (backup_fd);
		fatal_parsing_exit(parser, NULL, *line, MALLOC);
	}
	free (*line);
	free (temp);
	*line = new_line;
	new_line = NULL;
	temp = NULL;
	*line = check_pipes(*line, parser, 0, status);
	close (backup_fd);
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
