#include "minishell.h"

void	heredoc_mkdir(char **envp, t_pipes *my_pipes)
{
	pid_t	mkdir_pid;
	int		status;
	char	*mkdir_cmd[3];
	char	*mkdir_path;

	if (!my_pipes->paths)
		my_pipes->paths = get_paths(my_pipes);
	mkdir_pid = fork();
	if (mkdir_pid == 0)
	{
		mkdir_path = get_absolute_path(my_pipes->paths, "mkdir");
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

void	heredoc_rm(char **envp, t_pipes *my_pipes)
{
	pid_t	rm_pid;
	char	*rm_cmd[3];
	char	*rm_path;
	int		status;

	rm_pid = fork();
	if (rm_pid == 0)
	{
		rm_path = get_absolute_path(my_pipes->paths, "rm");
		rm_cmd[0] = "rm";
		rm_cmd[1] = "tmpfile";
		rm_cmd[2] = NULL;
		execve(rm_path, rm_cmd, envp);
	}
	waitpid(rm_pid, &status, 0);
}

void	heredoc_rmdir(char **envp, t_pipes *my_pipes)
{
	pid_t	rmdir_pid;
	char	*rmdir_cmd[3];
	char	*rmdir_path;
	int		status;

	rmdir_pid = fork();
	if (rmdir_pid == 0)
	{
		rmdir_path = get_absolute_path(my_pipes->paths, "rmdir");
		rmdir_cmd[0] = "rmdir";
		rmdir_cmd[1] = "tmp";
		rmdir_cmd[2] = NULL;
		execve(rmdir_path, rmdir_cmd, envp);
	}
	waitpid(rmdir_pid, &status, 0);
}
