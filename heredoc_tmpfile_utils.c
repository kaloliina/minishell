#include "minishell.h"

void	check_rm_success(t_pipes *my_pipes, pid_t pid, bool rm)
{
	int	status;

	status = 0;
	if (waitpid(pid, &status, 0) < 0)
	{
		my_pipes->hd_dir = 0;
		close (my_pipes->infile_fd);
		my_pipes->infile_fd = -1;
		handle_fatal_exit(ERR_WAITPID, my_pipes, NULL, NULL);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status))
	{
		my_pipes->exit_status = WEXITSTATUS(status);
		if (rm)
			chdir("..");
		my_pipes->hd_dir = 0;
		close (my_pipes->infile_fd);
		my_pipes->infile_fd = -1;
		handle_fatal_exit(NULL, my_pipes, NULL, NULL);
	}
}

void	check_tmp_dir(t_pipes *my_pipes)
{
	if (chdir("./tmp") == -1)
	{
		if (errno == ENOENT)
		{
			heredoc_mkdir(*my_pipes->my_envp, my_pipes, 0);
			my_pipes->hd_dir = 2;
		}
	}
	else
		my_pipes->hd_dir = 1;
}
