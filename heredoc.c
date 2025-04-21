#include "minishell.h"

static int	heredoc_read(t_node *delimiter_node,
	t_pipes *my_pipes, int status, int fd)
{
	char	*line;
	char	*temp;
	int		fd_backup;

	// fd_backup = dup(STDIN_FILENO); do we need this or do we have stdin in my_pipes always?
	signal(SIGINT, heredoc_signal);
	while (1)
	{
		line = readline("> ");
		if (g_signum == SIGINT)
		{
			g_signum = 0;
			dup2(my_pipes->stdinfd, STDIN_FILENO);
			// close (my_pipes->stdinfd); does this work like this or no?
			free (line);
			close (fd);
			return (-1);
		}
		if (!line)
		{
			ft_printf(2, "minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
			delimiter_node->delimiter);
			break ;
		}
		if (!ft_strcmp(line, delimiter_node->delimiter))
			break ;
		if (!delimiter_node->delimiter_quote)
		{
			temp = expand_heredoc(line, my_pipes, fd, status);
			if (temp)
				line = temp;
		}
		ft_printf(fd, "%s\n", line);
		free (line);
	}
	if (line)
		free (line);
	close (fd);
	my_pipes->infile_fd = open("tmpfile", O_RDONLY);
	//error checks
	return (0);
}

int	heredoc(t_node *curr, t_pipes *my_pipes,
	char **paths, int status)
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
			heredoc_mkdir(*my_pipes->my_envp, paths);
		}
	}
	fd = open("tmpfile", O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (fd < 0)
	{
		perror("minishell: tmpfile (heredoc temporary file)");
		return (-1);
	}
	flag = heredoc_read(curr, my_pipes, status, fd);
	heredoc_rm(*my_pipes->my_envp, paths);
	chdir("..");
	if (newdir)
		heredoc_rmdir(*my_pipes->my_envp, paths);
	return (flag);
}
