#include "minishell.h"

void	check_rmdir_success(t_pipes *my_pipes, pid_t pid)
{
	int	status;

	status = 0;
	if (waitpid(pid, &status, 0) < 0)
	{
		my_pipes->hd_dir = 0;
		if (my_pipes->infile_fd != -1
			&& close(my_pipes->infile_fd) < 0)
			print_error(ERR_CLOSE, NULL, NULL);
		else
			my_pipes->infile_fd = -1;
		fatal_exec_error(ERR_WAITPID, my_pipes, NULL, NULL);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status))
	{
		my_pipes->exit_status = WEXITSTATUS(status);
		my_pipes->hd_dir = 0;
		if (my_pipes->infile_fd != -1
			&& close(my_pipes->infile_fd) < 0)
			print_error(ERR_CLOSE, NULL, NULL);
		else
			my_pipes->infile_fd = -1;
		fatal_exec_error(NULL, my_pipes, NULL, NULL);
	}
}

void	check_tmp_dir(t_pipes *my_pipes)
{
	if (chdir("./tmp") < 0)
	{
		if (errno == ENOENT)
		{
			heredoc_mkdir(*my_pipes->my_envp, my_pipes, 0);
			my_pipes->hd_dir = 2;
		}
		else
			perror("minishell: chdir");
	}
	else
		my_pipes->hd_dir = 1;
}
