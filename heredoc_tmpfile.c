#include "minishell.h"

void	execve_fail_hd(t_pipes *my_pipes, char *msg)
{
	if (errno == ENOENT)
	{
		my_pipes->exit_status = 127;
		fatal_exec_error(ERR_INVFILE, my_pipes,
			NULL, msg);
	}
	else if (errno == EACCES)
	{
		my_pipes->exit_status = 126;
		fatal_exec_error(ERR_INVPERMS, my_pipes,
			NULL, msg);
	}
	else if (errno == ENOEXEC)
	{
		my_pipes->exit_status = 1;
		fatal_exec_error(ERR_FORMAT, my_pipes,
			NULL, msg);
	}
	else
	{
		my_pipes->exit_status = 1;
		fatal_exec_error(ERR_EXECVE, my_pipes,
			NULL, msg);
	}
}

static void	heredoc_mkdir_child(t_pipes *my_pipes, char **envp)
{
	char	*mkdir_cmd[3];
	char	*mkdir_path;

	close_all_fds(my_pipes);
	mkdir_path = get_absolute_path(my_pipes->paths, "mkdir", my_pipes);
	mkdir_cmd[0] = "mkdir";
	mkdir_cmd[1] = "tmp";
	mkdir_cmd[2] = NULL;
	execve(mkdir_path, mkdir_cmd, envp);
	free (mkdir_path);
	execve_fail_hd(my_pipes, "mkdir");
}

void	heredoc_mkdir(char **envp, t_pipes *my_pipes, int status)
{
	pid_t	mkdir_pid;

	(void)envp;
	if (!my_pipes->paths)
		my_pipes->paths = get_paths(my_pipes);
	mkdir_pid = fork();
	if (mkdir_pid < 0)
		fatal_exec_error(ERR_FORK, my_pipes, NULL, NULL);
	if (mkdir_pid == 0)
		heredoc_mkdir_child(my_pipes, envp);
	if (waitpid(mkdir_pid, &status, 0) < 0)
		fatal_exec_error(ERR_WAITPID, my_pipes, NULL, NULL);
	if (WIFEXITED(status) && WEXITSTATUS(status))
	{
		my_pipes->exit_status = WEXITSTATUS(status);
		fatal_exec_error(NULL, my_pipes, NULL, NULL);
	}
	if (chdir("./tmp") < 0)
		perror("minishell: chdir");
}

void	heredoc_rmdir(char **envp, t_pipes *my_pipes)
{
	pid_t	rmdir_pid;
	char	*rmdir_cmd[3];
	char	*rmdir_path;
	int		status;

	status = 0;
	rmdir_pid = fork();
	if (rmdir_pid < 0)
		fatal_exec_error(ERR_FORK, my_pipes, NULL, NULL);
	if (rmdir_pid == 0)
	{
		close_all_fds(my_pipes);
		rmdir_path = get_absolute_path(my_pipes->paths, "rmdir", my_pipes);
		rmdir_cmd[0] = "rmdir";
		rmdir_cmd[1] = "tmp";
		rmdir_cmd[2] = NULL;
		execve(rmdir_path, rmdir_cmd, envp);
		my_pipes->hd_dir = 0;
		free (rmdir_path);
		execve_fail_hd(my_pipes, "rmdir");
	}
	check_rmdir_success(my_pipes, rmdir_pid);
	my_pipes->hd_dir = 0;
}

void	handle_tmpfile(t_pipes *my_pipes)
{
	if (my_pipes->heredoc_node->hd_fd != -1)
	{
		if (close(my_pipes->heredoc_node->hd_fd) < 0)
			print_error(ERR_CLOSE, NULL, NULL);
		else
			my_pipes->heredoc_node->hd_fd = -1;
	}
	if (unlink("minishell_tmpfile") < 0)
	{
		my_pipes->hd_dir = 0;
		fatal_exec_error(ERR_UNLINK, my_pipes, NULL, NULL);
	}
	if (chdir("..") < 0)
		perror("minishell: chdir");
	if (my_pipes->hd_dir == 2)
		heredoc_rmdir(*my_pipes->my_envp, my_pipes);
	my_pipes->hd_dir = 0;
}
