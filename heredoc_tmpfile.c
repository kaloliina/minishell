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
		my_pipes->exit_status = 0;
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

void	heredoc_mkdir(char **envp, t_pipes *my_pipes, int status)
{
	pid_t	mkdir_pid;
	char	*mkdir_cmd[3];
	char	*mkdir_path;

	if (!my_pipes->paths)
		my_pipes->paths = get_paths(my_pipes);
	mkdir_pid = fork();
	if (mkdir_pid == 0)
	{
		mkdir_path = get_absolute_path(my_pipes->paths, "mkdir", my_pipes);
		mkdir_cmd[0] = "mkdir";
		mkdir_cmd[1] = "tmp";
		mkdir_cmd[2] = NULL;
		execve(mkdir_path, mkdir_cmd, envp);
		execve_fail_hd(my_pipes, "execve");
	}
	if (waitpid(mkdir_pid, &status, 0) < 0)
		fatal_exec_error(ERR_WAITPID, my_pipes, NULL, NULL);
	if (WIFEXITED(status) && WEXITSTATUS(status))
	{
		my_pipes->exit_status = WEXITSTATUS(status);
		fatal_exec_error(NULL, my_pipes, NULL, NULL);
	}
	chdir("./tmp");
}

int	heredoc_rm(char **envp, t_pipes *my_pipes)
{
	pid_t	rm_pid;
	char	*rm_cmd[3];
	char	*rm_path;

	rm_pid = fork();
	if (rm_pid == 0)
	{
		rm_path = get_absolute_path(my_pipes->paths, "rm", my_pipes);
		rm_cmd[0] = "rm";
		rm_cmd[1] = "minishell_tmpfile";
		rm_cmd[2] = NULL;
		execve(rm_path, rm_cmd, envp);
		my_pipes->hd_dir = 0;
		close (my_pipes->infile_fd);
		my_pipes->infile_fd = -1;
		execve_fail_hd(my_pipes, "execve");
	}
	return (rm_pid);
}

void	heredoc_rmdir(char **envp, t_pipes *my_pipes)
{
	pid_t	rmdir_pid;
	char	*rmdir_cmd[3];
	char	*rmdir_path;
	int		status;

	status = 0;
	rmdir_pid = fork();
	if (rmdir_pid == 0)
	{
		rmdir_path = get_absolute_path(my_pipes->paths, "rmdir", my_pipes);
		rmdir_cmd[0] = "rmdir";
		rmdir_cmd[1] = "tmp";
		rmdir_cmd[2] = NULL;
		execve(rmdir_path, rmdir_cmd, envp);
		my_pipes->hd_dir = 0;
		close (my_pipes->infile_fd);
		my_pipes->infile_fd = -1;
		execve_fail_hd(my_pipes, "execve");
	}
	check_rm_success(my_pipes, rmdir_pid, 0);
	my_pipes->hd_dir = 0;
}

void	handle_tmpfile(t_pipes *my_pipes)
{
	pid_t	rm_pid;
	int		status;

	status = 0;
	rm_pid = heredoc_rm(*my_pipes->my_envp, my_pipes);
	if (waitpid(rm_pid, &status, 0) < 0)
		fatal_exec_error(ERR_WAITPID, my_pipes, NULL, NULL);
	if (WIFEXITED(status) && WEXITSTATUS(status))
	{
		my_pipes->exit_status = WEXITSTATUS(status);
		close (my_pipes->infile_fd);
		my_pipes->infile_fd = -1;
		fatal_exec_error(NULL, my_pipes, NULL, NULL);
	}
	chdir("..");
	if (my_pipes->hd_dir == 2)
		heredoc_rmdir(*my_pipes->my_envp, my_pipes);
	my_pipes->hd_dir = 0;
}
