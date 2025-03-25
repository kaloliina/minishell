#include "minishell.h"

void	heredoc(t_node *node, t_pipes *my_pipes, char **envp, char **paths)
{
	int		fd;
	int		pid;
	char	*path;
	char	*line;
	char	**rm_cmd;
	t_node	*delimiter_node;

	rm_cmd = NULL;
	delimiter_node = node;
	path = get_absolute_path(paths, "rm");
	while (!delimiter_node->delimiter)
		delimiter_node = delimiter_node->prev;
	fd = open("tmpfile", O_CREAT | O_WRONLY, 0777);
	while (1)
	{
		line = readline("> ");
		if (!ft_strcmp(line, delimiter_node->delimiter))
			break ;
		ft_putendl_fd(line, fd);
		free (line);
	}
	if (line)
		free (line);
	close (fd);
	fd = open("tmpfile", O_RDONLY);
	my_pipes->stdinfd = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close (fd);
	pid = fork();
	if (pid == 0)
	{
		rm_cmd = malloc(sizeof(char *) * 3);
		rm_cmd[0] = "rm";
		rm_cmd[1] = "tmpfile";
		rm_cmd[2] = NULL;
		execve(path, rm_cmd, envp);
	}
}

// void	heredoc(t_node *node, t_pipes *my_pipes, char **envp, char *path)
// {
// 	int		hd_pid;
// 	int		hd_fd[2];
// 	char	*line;
// 	t_node	*delimiter_node;

// 	delimiter_node = node;
// 	while (!delimiter_node->delimiter)
// 		delimiter_node = delimiter_node->prev;
// 	pipe(hd_fd);
// 	hd_pid = fork();
// 	if (hd_pid == 0)
// 	{
// 		close (hd_fd[0]);
// 		while (1)
// 		{
// 			line = readline("> ");
// 			if (!ft_strcmp(line, delimiter_node->delimiter))
// 				break ;
// 			ft_putendl_fd(line, hd_fd[1]);
// 			free (line);
// 		}
// 		if (line)
// 			free (line);
// 		my_pipes->stdinfd = dup(STDIN_FILENO);
// 		dup2(hd_fd[1], STDIN_FILENO);
// 		close(hd_fd[1]);
// 		exit (0);
// 	}
// 	else
// 	{
// 		int	status;
// 		wait(&status);
// 		close (hd_fd[1]);
// 		close (hd_fd[0]);
// 	}
// }
