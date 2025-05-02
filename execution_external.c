#include "../minishell.h"

static int	prep_for_execution(t_pipes *my_pipes)
{
	struct stat	sb;

	if (my_pipes->paths == NULL)
		my_pipes->paths = get_paths(my_pipes);
	if (!ft_strcmp(my_pipes->command_node->cmd[0], "")
		&& !my_pipes->command_node->cmd[1])
		return (-1);
	my_pipes->command_path = get_absolute_path(my_pipes->paths,
			my_pipes->command_node->cmd[0], my_pipes);
	if (my_pipes->command_path == NULL)
	{
		ft_printf(2, "minishell: ");
		ft_printf(2, ERR_COMMAND, my_pipes->command_node->cmd[0]);
		my_pipes->exit_status = 127;
		return (-1);
	}
	if (stat(my_pipes->command_path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		ft_printf(2, "minishell: ");
		ft_printf(2, ERR_DIR, my_pipes->command_node->cmd[0]);
		my_pipes->exit_status = 126;
		return (-1);
	}
	return (0);
}

//if file does not have #!/bin/bash, in bash it still attempts to add
//the path so this might need to be tweaked a bit
//also exit status 0 is unclear
static void	handle_execve_errors(t_pipes *my_pipes)
{
	if (errno == ENOENT)
	{
		my_pipes->exit_status = 127;
		fatal_exec_error(ERR_INVFILE, my_pipes,
			NULL, my_pipes->command_node->cmd[0]);
	}
	else if (errno == EACCES)
	{
		my_pipes->exit_status = 126;
		fatal_exec_error(ERR_INVPERMS, my_pipes,
			NULL, my_pipes->command_node->cmd[0]);
	}
	else if (errno == ENOEXEC)
	{
		my_pipes->exit_status = 1;
		fatal_exec_error(ERR_FORMAT, my_pipes,
			NULL, my_pipes->command_node->cmd[0]);
	}
	else
	{
		my_pipes->exit_status = 1;
		fatal_exec_error(ERR_EXECVE, my_pipes,
			NULL, my_pipes->command_node->cmd[0]);
	}
}

//do I even need to close all fds here? execve closes them anyway..?
int	execute_executable(t_node *node, t_pipes *my_pipes)
{
	int	pid;

	if (prep_for_execution(my_pipes) < 0)
		return (0);
	pid = fork();
	if (pid < 0)
		fatal_exec_error(ERR_FORK, my_pipes, NULL, NULL);
	if (pid == 0)
	{
		listen_to_signals(0);
		handle_redirections(my_pipes);
		close_all_fds(my_pipes);
		if (my_pipes->exit_status == 1)
			exit(1);
		execve(my_pipes->command_path, &node->cmd[0], *(my_pipes->my_envp));
		handle_execve_errors(my_pipes);
	}
	else
	{
		listen_to_signals(1);
	}
	return (pid);
}
