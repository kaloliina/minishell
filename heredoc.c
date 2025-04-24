#include "minishell.h"

static int	heredoc_sigint(t_pipes *my_pipes, char *line, int fd)
{
	g_signum = 0;
	dup2(my_pipes->stdinfd, STDIN_FILENO);
	close (my_pipes->stdinfd);
	my_pipes->stdinfd = -1;
	free (line);
	close (fd);
	return (-1);
}

static void	heredoc_expand(char **line, t_pipes *my_pipes, int fd, int status)
{
	char	*temp;

	temp = expand_heredoc(*line, my_pipes, fd, status);
	if (temp)
		*line = temp;
}

static void	heredoc_free_close(char *line, int fd)
{
	if (line)
		free (line);
	close (fd);
}

static int	heredoc_read(t_node *delimiter_node,
	t_pipes *my_pipes, int status, int fd)
{
	char	*line;
	char	*temp;
	int		fd_backup;

	while (1)
	{
		signal(SIGINT, heredoc_signal);
		line = readline("> ");
		if (g_signum == SIGINT)
			return (heredoc_sigint(my_pipes, line, fd));
		if (!line)
		{
			ft_printf(2, HD_CTRLD, delimiter_node->delimiter);
			break ;
		}
		if (!ft_strcmp(line, delimiter_node->delimiter))
			break ;
		if (!delimiter_node->delimiter_quote)
			heredoc_expand(&line, my_pipes, fd, status);
		ft_printf(fd, "%s\n", line);
		free (line);
	}
	heredoc_free_close(line, fd);
	open_infile("tmpfile", my_pipes);
	return (0);
}

int	heredoc(t_node *curr, t_pipes *my_pipes, int status)
{
	int	newdir;
	int	fd;
	int	flag;

	newdir = 0;
	if (chdir("./tmp") == -1)
	{
		if (errno == ENOENT)
		{
			newdir = 1;
			heredoc_mkdir(*my_pipes->my_envp, my_pipes);
		}
	}
	fd = open("tmpfile", O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (fd < 0)
	{
		perror("minishell: tmpfile (here-document)");
		return (-1);
	}
	flag = heredoc_read(curr, my_pipes, status, fd);
	heredoc_rm(*my_pipes->my_envp, my_pipes);
	chdir("..");
	if (newdir)
		heredoc_rmdir(*my_pipes->my_envp, my_pipes);
	return (flag);
}
