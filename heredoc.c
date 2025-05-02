#include "minishell.h"

static int	heredoc_sigint(t_pipes *my_pipes, char *line, int fd)
{
	g_signum = 0;
	dup2(my_pipes->stdinfd, STDIN_FILENO);
	close (my_pipes->stdinfd);
	my_pipes->stdinfd = -1;
	my_pipes->exit_status = 130;
	free (line);
	close (fd);
	return (-1);
}

static void	heredoc_free_close(char *line, int fd)
{
	if (line)
		free (line);
	close (fd);
}

static void	expand_heredoc(char **line, t_pipes *my_pipes, int status,
	t_node *heredoc_node)
{
	int		i;
	t_exp	expand;

	i = 0;
	init_exp(&expand, status, NULL, my_pipes);
	expand.new_line = ft_strdup("");
	if (!expand.new_line)
	{
		my_pipes->exit_status = 1;
		fatal_exec_error(ERR_MALLOC, my_pipes, my_pipes->heredoc_node, NULL);
	}
	while ((*line)[i])
	{
		if ((*line)[i] == '$' && (*line)[i + 1])
			i = expand_line_helper(*line, &expand.new_line, &expand, i + 1);
		else
		{
			append_char_heredoc(&expand.new_line, (*line)[i],
				my_pipes, heredoc_node);
			i++;
		}
	}
	*line = expand.new_line;
}

static int	heredoc_read(t_node *heredoc_node,
	t_pipes *my_pipes, int status)
{
	char	*line;

	while (1)
	{
		signal(SIGINT, heredoc_signal);
		line = readline("> ");
		if (g_signum == SIGINT)
			return (heredoc_sigint(my_pipes, line, heredoc_node->hd_fd));
		if (!line)
		{
			print_error(ERR_HD, NULL, NULL);
			print_error(ERR_HD_DLM, heredoc_node->delimiter, NULL);
			break ;
		}
		if (!ft_strcmp(line, heredoc_node->delimiter))
			break ;
		if (!heredoc_node->delimiter_quote)
			expand_heredoc(&line, my_pipes, status, heredoc_node);
		ft_printf(heredoc_node->hd_fd, "%s\n", line);
		free (line);
	}
	heredoc_free_close(line, heredoc_node->hd_fd);
	open_infile("minishell_tmpfile", my_pipes);
	return (0);
}

int	heredoc(t_node *heredoc_node, t_pipes *my_pipes, int status)
{
	int		flag;
	pid_t	rm_pid;

	check_tmp_dir(my_pipes);
	heredoc_node->hd_fd
		= open("minishell_tmpfile", O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (heredoc_node->hd_fd < 0)
	{
		perror("minishell: minishell_tmpfile (here-document)");
		return (-1);
	}
	flag = heredoc_read(heredoc_node, my_pipes, status);
	rm_pid = heredoc_rm(*my_pipes->my_envp, my_pipes);
	check_rm_success(my_pipes, rm_pid, 1);
	chdir("..");
	if (my_pipes->hd_dir == 2)
		heredoc_rmdir(*my_pipes->my_envp, my_pipes);
	return (flag);
}
