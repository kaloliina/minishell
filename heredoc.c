#include "minishell.h"
#include <errno.h>

static void	heredoc_rm(char **envp, char **paths)
{
	pid_t	rm_pid;
	char	**rm_cmd;
	char	*rm_path;

	rm_pid = fork();
	if (rm_pid == 0)
	{
		rm_path = get_absolute_path(paths, "rm");
		rm_cmd = malloc(sizeof(char *) * 3);
		if (!rm_cmd)
		{
			ft_putstr_fd("minishell: memory allocation failure\n", 2);
			exit (1);
		}
		rm_cmd[0] = "rm";
		rm_cmd[1] = "tmpfile";
		rm_cmd[2] = NULL;
		execve(rm_path, rm_cmd, envp);
	}
}

static void	heredoc_rmdir(char **envp, char **paths)
{
	pid_t	rmdir_pid;
	char	**rmdir_cmd;
	char	*rmdir_path;

	rmdir_pid = fork();
	if (rmdir_pid == 0)
	{
		rmdir_path = get_absolute_path(paths, "rmdir");
		rmdir_cmd = malloc(sizeof(char *) * 3);
		if (!rmdir_cmd)
		{
			ft_putstr_fd("minishell: memory allocation failure\n", 2);
			exit (1);
		}
		rmdir_cmd[0] = "rmdir";
		rmdir_cmd[1] = "tmp";
		rmdir_cmd[2] = NULL;
		execve(rmdir_path, rmdir_cmd, envp);
	}
}

static void	heredoc_read(t_node *delimiter_node, t_pipes *my_pipes)
{
	int		fd;
	char	*line;

	fd = open("tmpfile", O_CREAT | O_TRUNC | O_WRONLY, 0777);
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
}

static void	heredoc_mkdir(char **envp, char **paths)
{
	pid_t	mkdir_pid;
	int		status;
	char	**mkdir_cmd;
	char	*mkdir_path;

	mkdir_pid = fork();
	if (mkdir_pid == 0)
	{
		mkdir_path = get_absolute_path(paths, "mkdir");
		mkdir_cmd = malloc(sizeof(char *) * 3);
		if (!mkdir_cmd)
		{
			ft_putstr_fd("minishell: memory allocation failure\n", 2);
			exit (1);
		}
		mkdir_cmd[0] = "mkdir";
		mkdir_cmd[1] = "tmp";
		mkdir_cmd[2] = NULL;
		execve(mkdir_path, mkdir_cmd, envp);
	}
	waitpid(mkdir_pid, &status, 0);
	if (status == 1)
	{
		//free everything
		exit (1);
	}
	chdir("./tmp");
}

void	heredoc(t_node *node, t_pipes *my_pipes, char **envp, char **paths)
{
	int		fd;
	int		newdir;
	char	*line;
	t_node	*delimiter_node;

	newdir = 0;
	delimiter_node = node;
	while (!delimiter_node->delimiter)
		delimiter_node = delimiter_node->prev;
	if (chdir("./tmp") == -1)
	{
		if (errno == ENOENT)
		{
			newdir = 1;
			heredoc_mkdir(envp, paths);
		}
	}
	heredoc_read(delimiter_node, my_pipes);
	heredoc_rm(envp, paths);
	chdir("..");
	if (newdir)
		heredoc_rmdir(envp, paths);
}
