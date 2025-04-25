#include "../minishell.h"

//This function needs renaming
void	close_all_pipes(t_pipes *my_pipes)
{
	int	i;

	i = 0;
	while (my_pipes->pipes && i < my_pipes->pipe_amount * 2)
	{
		if (my_pipes->pipes[i] != -1)
		{
			if (close(my_pipes->pipes[i]) < 0)
				ft_printf(2, "%s\n", ERR_CLOSE);
			my_pipes->pipes[i] = -1;
		}
		i++;
	}
	if (my_pipes->stdinfd != -1 && (close(my_pipes->stdinfd) < 0))
		ft_printf(2, "%s\n", ERR_CLOSE);
	my_pipes->stdinfd = -1;
	if (my_pipes->stdoutfd != -1 && (close(my_pipes->stdoutfd) < 0))
		ft_printf(2, "%s\n", ERR_CLOSE);
	my_pipes->stdoutfd = -1;
	if (my_pipes->outfile_fd != -1 && (close(my_pipes->outfile_fd) < 0))
		ft_printf(2, "%s\n", ERR_CLOSE);
	my_pipes->outfile_fd = -1;
	if (my_pipes->infile_fd != -1 && (close(my_pipes->infile_fd) < 0))
		ft_printf(2, "%s\n", ERR_CLOSE);
	my_pipes->infile_fd = -1;
}

void	free_my_pipes(t_pipes *my_pipes)
{
	if (my_pipes)
	{
		if (my_pipes->paths)
			free_array(my_pipes->paths);
		if (my_pipes->command_path)
		{
			free(my_pipes->command_path);
			my_pipes->command_path = NULL;
		}
		close_all_pipes(my_pipes);
		if (my_pipes->pipes)
		{
			free(my_pipes->pipes);
			my_pipes->pipes = NULL;
		}
		if (my_pipes->childpids)
		{
			free(my_pipes->childpids);
			my_pipes->childpids = NULL;
		}
		free(my_pipes);
	}
}

int	get_pipe_amount(t_node *list)
{
	t_node	*curr;
	int		pipe_amount;

	pipe_amount = 0;
	curr = list;
	while (curr)
	{
		if (curr->type == PIPE)
			pipe_amount++;
		curr = curr->next;
	}
	return (pipe_amount);
}

/*Just double check the if statement here
 there was a reason why you put it like that
printf("Moving to next pipe: read_end = %d, write_end = %d\n",
 my_pipes->read_end, my_pipes->write_end);
printf("Curr section: %d\n", my_pipes->current_section);*/
void	reset_properties(t_pipes *my_pipes)
{
	if (my_pipes->command_path != NULL
		&& ft_strcmp(my_pipes->command_path, my_pipes->command_node->cmd[0]))
	{
		free (my_pipes->command_path);
		my_pipes->command_path = NULL;
	}
	my_pipes->command_node = NULL;
	my_pipes->exit_status = 0;
	if (my_pipes->current_section != (my_pipes->pipe_amount + 1))
	{
		my_pipes->read_end = my_pipes->write_end - 1;
		if (my_pipes->current_section < my_pipes->pipe_amount)
			my_pipes->write_end = my_pipes->write_end + 2;
	}
	my_pipes->current_section++;
}

//Double check the exit status check here
void	handle_redirections(t_node *node, t_pipes *my_pipes)
{
	if (my_pipes->exit_status == 1)
		return ;
	my_pipes->exit_status = 0;
	if (my_pipes->outfile_fd >= 0
		&& (dup2(my_pipes->outfile_fd, STDOUT_FILENO) < 0))
		my_pipes->exit_status = 1;
	if (my_pipes->infile_fd >= 0
		&& (dup2(my_pipes->infile_fd, STDIN_FILENO) < 0))
		my_pipes->exit_status = 1;
	if (my_pipes->current_section != 1 && my_pipes->infile_fd == -1)
	{
		if (dup2(my_pipes->pipes[my_pipes->read_end], STDIN_FILENO) < 0)
			my_pipes->exit_status = 1;
	}
	if ((my_pipes->current_section != my_pipes->pipe_amount + 1)
		&& my_pipes->outfile_fd == -1)
	{
		if (dup2(my_pipes->pipes[my_pipes->write_end], STDOUT_FILENO) < 0)
			my_pipes->exit_status = 1;
	}
	if (my_pipes->exit_status == 1)
		ft_printf(2, "%s\n", ERR_FD);
}

/*Function to close pipes in the parent.
- We close always the write end except for the last section because that
 end was already closed
- We close read always after we reach the second section, not in the first
 section because we still need it
- Then in the end we reset properties as well as go over to the next pair
 or pipe ends.
//		printf("Current section: %d\n", my_pipes->current_section);
//			printf("Closing write end: %d\n", my_pipes->write_end);
//			printf("Closing read end: %d\n", my_pipes->read_end);
This function needs renaming!
*/
void	close_pipes(t_pipes *my_pipes)
{
	if (my_pipes->pipe_amount > 0)
	{
		if (my_pipes->current_section <= my_pipes->pipe_amount)
		{
			if (close(my_pipes->pipes[my_pipes->write_end]) < 0)
				ft_printf(2, "%s\n", ERR_CLOSE);
			my_pipes->pipes[my_pipes->write_end] = -1;
		}
		if (my_pipes->current_section != 1)
		{
			if (close(my_pipes->pipes[my_pipes->read_end]) < 0)
				ft_printf(2, "%s\n", ERR_CLOSE);
			my_pipes->pipes[my_pipes->read_end] = -1;
		}
	}
	if (my_pipes->outfile_fd != -1 && (close(my_pipes->outfile_fd) < 0))
		ft_printf(2, "%s\n", ERR_CLOSE);
	my_pipes->outfile_fd = -1;
	if (my_pipes->infile_fd != -1 && (close(my_pipes->infile_fd) < 0))
		ft_printf(2, "%s\n", ERR_CLOSE);
	my_pipes->infile_fd = -1;
	if (my_pipes->current_section != (my_pipes->pipe_amount + 1))
		reset_properties(my_pipes);
}

void	run_builtin_command(t_node *node, t_pipes *my_pipes)
{
	if (!ft_strcmp(node->cmd[0], "echo"))
		execute_echo(node, my_pipes->my_envp);
	else if (!ft_strcmp(node->cmd[0], "pwd"))
		execute_pwd(my_pipes);
	else if (!ft_strcmp(node->cmd[0], "env"))
		execute_env(my_pipes->my_envp);
	else if (!ft_strcmp(node->cmd[0], "export"))
		execute_export(node->cmd, my_pipes->my_envp, my_pipes);
	else if (!ft_strcmp(node->cmd[0], "unset"))
		execute_unset(node->cmd, my_pipes->my_envp, my_pipes);
	else if (!ft_strcmp(node->cmd[0], "exit"))
		execute_exit(node->cmd, my_pipes);
	else if (!ft_strcmp(node->cmd[0], "cd"))
		execute_cd(node->cmd, my_pipes);
}

void	listen_to_signals(int in_parent)
{
	if (in_parent == 1)
	{
		signal(SIGQUIT, parent_signal);
		signal(SIGINT, parent_signal);
	//	my_pipes->exit_status = g_signum + 128;	//can we do this or does it work wrong if signal is not received?
		g_signum = 0;//must we always do this so old signal is not stored for later		
	}
	else
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);	
	}

}

void	execute_builtin_child(t_node *node, t_pipes *my_pipes)
{
	listen_to_signals(0);
	handle_redirections(node, my_pipes);
	if (my_pipes->exit_status == 1)
	{
		free_my_pipes(my_pipes);
		exit (1);
	}
	run_builtin_command(node, my_pipes);
	free_my_pipes(my_pipes);
	exit(0);	
}

int	execute_builtin(t_node *node, t_pipes *my_pipes)
{
	int	pid;

	if (my_pipes->pipe_amount > 0)
	{
		pid = fork();
		if (pid < 0)
			handle_fatal_exit(ERR_FORK, my_pipes, NULL, NULL);
		if (pid == 0)
			execute_builtin_child(node, my_pipes);
		else
		{
			listen_to_signals(1);
			return (pid);
		}
	}
	else
	{
		handle_redirections(node, my_pipes);
		if (my_pipes->exit_status == 1)
			return (0);
		run_builtin_command(node, my_pipes);
		return (0);
	}
}

// Returns: 1=directory, 0=file, -1=error (errno set)
int	prep_for_execution(t_pipes *my_pipes)
{
	struct stat	sb;

	if (my_pipes->paths == NULL)
		my_pipes->paths = get_paths(my_pipes);
	if (!ft_strcmp(node->cmd[0], "") && !node->cmd[1])
		return (-1);
	my_pipes->command_path = get_absolute_path(my_pipes->paths, my_pipes->command_node->cmd[0]);
	if (my_pipes->command_path == NULL)
	{
		ft_printf(2, ERR_COMMAND, my_pipes->command_node->cmd[0]);
		my_pipes->exit_status = 127;
		return (-1);
	}
	if (stat(my_pipes->command_path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		ft_printf(2, ERR_DIR, my_pipes->command_node->cmd[0]);
		my_pipes->exit_status = 126;
		return (-1);
	}
	return (0);
}

//if file does not have #!/bin/bash, in bash it still attempts to add
//  the path so this might need to be tweaked a bit
//also exit status 0 is unclear
void	handle_execve_errors(t_pipes *my_pipes)
{
	if (errno == ENOENT)
	{
		my_pipes->exit_status = 127;
		handle_fatal_exit(ERR_INVFILE, my_pipes, NULL, my_pipes->command_node->cmd[0]);
	}
	else if (errno == EACCES)
	{
		my_pipes->exit_status = 126;
		handle_fatal_exit(ERR_INVPERMS, my_pipes, NULL, my_pipes->command_node->cmd[0]);
	}
	else if (errno == ENOEXEC)
	{
		my_pipes->exit_status = 0;
		handle_fatal_exit(ERR_FORMAT, my_pipes, NULL, my_pipes->command_node->cmd[0]);
	}
	else
	{
		my_pipes->exit_status = 1;
		handle_fatal_exit(ERR_EXECVE, my_pipes, NULL, my_pipes->command_node->cmd[0]);
	}
}

/* EISDIR AND ENOEXEC behace unexpectedly.
Also it might be worthwhile to add these errno checks in a separate function.
*/
//make a function for the signals
int	execute_executable(t_node *node, t_pipes *my_pipes)
{
	int	pid;

	if (prep_for_execution(my_pipes) < 0)
		return (0);
	pid = fork();
	if (pid < 0)
		handle_fatal_exit(ERR_FORK, my_pipes, NULL, NULL);
	if (pid == 0)
	{
		listen_to_signals(0);
		handle_redirections(node, my_pipes);
		close_all_pipes(my_pipes);
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

int	is_builtin(char *command)
{
	const char	*builtins[7];
	int			i;

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	i = 0;
	while (i <= 6)
	{
		if (!ft_strcmp(builtins[i], command))
			return (1);
		i++;
	}
	return (0);
}

void	initialize_struct(t_pipes *my_pipes, t_node *list, char ***envp)
{
	int	i;

	*my_pipes = (t_pipes){NULL, NULL, get_pipe_amount(list), 1, -1, -1, -1, -1, 0, 1, 0, NULL, NULL, envp, NULL};
	if (my_pipes->pipe_amount > 0)
	{
		my_pipes->pipes = malloc(sizeof(int) * (my_pipes->pipe_amount * 2));
		if (my_pipes->pipes == NULL)
			handle_fatal_exit(MALLOC, my_pipes, list, NULL);
		i = 0;
		while (i < my_pipes->pipe_amount)
		{
			if (pipe(&my_pipes->pipes[i++ * 2]) < 0)
				handle_fatal_exit(ERR_PIPE, my_pipes, list, NULL);
		}
	}
	my_pipes->childpids = ft_calloc(sizeof(pid_t), (my_pipes->pipe_amount + 1));
	if (my_pipes->childpids == NULL)
		handle_fatal_exit(MALLOC, my_pipes, list, NULL);
	my_pipes->stdoutfd = dup(STDOUT_FILENO);
	if (my_pipes->stdoutfd == -1)
		handle_fatal_exit(ERR_FD, my_pipes, list, NULL);
	my_pipes->stdinfd = dup(STDIN_FILENO);
	if (my_pipes->stdinfd == -1)
		handle_fatal_exit(ERR_FD, my_pipes, list, NULL);
}

/*Double check exit statuses
Also there might be a situation with sleep where you explicitly have to mark the last process but I was unable to repro the behaviour
Make this shorter
*/
int	finalize_execution(int amount, t_pipes *my_pipes)
{
	int	status;
	int	exit_status;
	int	i;

	status = 0;
	exit_status = my_pipes->exit_status;
	i = 0;
	while (i < amount)
	{
		if (my_pipes->childpids[i] > 0)
		{
			if (waitpid(my_pipes->childpids[i], &status, 0) < 0)
				handle_fatal_exit(ERR_WAITPID, my_pipes, NULL, NULL);
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	if (dup2(my_pipes->stdinfd, STDIN_FILENO) < 0 || dup2(my_pipes->stdoutfd, STDOUT_FILENO) < 0)
		handle_fatal_exit(ERR_FD, my_pipes, NULL, NULL);
	free_my_pipes(my_pipes);
	return (exit_status);
}

//Shorten this
int	loop_nodes(t_node *list, char ***envp, int status)
{
	t_pipes	*my_pipes;

	my_pipes = malloc(sizeof(t_pipes));
	if (my_pipes == NULL)
	{
		free_array(*envp);
		handle_fatal_exit(MALLOC, my_pipes, list, NULL);
	}
	initialize_struct(my_pipes, list, envp);
	while (list != NULL)
	{
		if (list->type == COMMAND)
			my_pipes->command_node = list;
		if (list->file && *list->file && (list->type == REDIR_APPEND || list->type == REDIR_OUTF))
			set_outfile(list->file, list->type, my_pipes);
		if (list->file && *list->file && list->type == REDIR_INF)
			open_infile(list->file, my_pipes);
		if (list->type == REDIR_HEREDOC)
		{
			if (heredoc(list, my_pipes, my_pipes->paths, status) < 0)
			{
				free_my_pipes(my_pipes);
				return (130);
			}
		}
		if ((list->next == NULL) || (list->next && my_pipes->pipe_amount > 0 && list->next->type == PIPE))
		{
			if (my_pipes->command_node != NULL && is_builtin(my_pipes->command_node->cmd[0]) == 1)
				my_pipes->childpids[my_pipes->current_section -1] = execute_builtin(my_pipes->command_node, my_pipes);
			else if (my_pipes->command_node != NULL)
			{
				my_pipes->childpids[my_pipes->current_section -1] = execute_executable(my_pipes->command_node, my_pipes);
				if (my_pipes->childpids[my_pipes->current_section -1] == -1)
					return (finalize_execution(my_pipes->current_section, my_pipes));
			}
			close_pipes(my_pipes);
		}
		list = list->next;
	}
	return (finalize_execution(my_pipes->current_section, my_pipes));
}
