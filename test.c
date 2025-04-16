#include "../minishell.h"

/* CAN WE MAKE THIS SHORTER?
- Remove if my pipes is not null, feels like we never encounter a situation like that because it's checked beforehand
- Otherwise combine some stuff*/
void	free_my_pipes(t_pipes *my_pipes)
{
	int	i;

	i = 0;
	if (my_pipes)
	{
		if (my_pipes->paths)
			free_array(my_pipes->paths);
		if (my_pipes->command_path)
		{
			free(my_pipes->command_path);
			my_pipes->command_path = NULL;
		}
		if (my_pipes->stdinfd != -1 && (close(my_pipes->stdinfd) < 0))
			ft_printf(2, "%s\n", ERR_CLOSE);
		if (my_pipes->stdoutfd != -1 && (close(my_pipes->stdoutfd) < 0))
			ft_printf(2, "%s\n", ERR_CLOSE);
		if (my_pipes->pipes)
		{
			while (i < my_pipes->pipe_amount * 2)
			{
				if (my_pipes->pipes[i] != -1)
				{
					if (close(my_pipes->pipes[i]) < 0)
						ft_printf(2, "%s\n", ERR_CLOSE);
				}
				i++;
			}
			free(my_pipes->pipes);
			my_pipes->pipes = NULL;
		}
		free(my_pipes);
	}
}

static void	handle_fatal_exit(char *msg, t_pipes *my_pipes, t_node *list, char *conversion)
{
	if (conversion == NULL)
		ft_printf(2, msg);
	else
		ft_printf(2, msg, conversion);
	if (list == NULL)
		free_nodes(my_pipes->command_node);
	else
		free_nodes(list);
	if (my_pipes != NULL)
	{
		free_array(*my_pipes->my_envp);
		free_my_pipes(my_pipes);
	}
	exit (my_pipes->exit_status);
}

static void	redirection_outfile(t_pipes *my_pipes)
{
	if (dup2(my_pipes->outfile_fd, STDOUT_FILENO) < 0)
	{
		ft_printf(2, "%s\n", ERR_FD);
		my_pipes->exit_status = 1;
	}
	if (close(my_pipes->outfile_fd) < 0)
		ft_printf(2, "%s\n", ERR_CLOSE);
}

static void	redirection_infile(t_pipes *my_pipes)
{
	if (dup2(my_pipes->infile_fd, STDIN_FILENO) < 0)
	{
		ft_printf(2, "%s\n", ERR_FD);
		my_pipes->exit_status = 1;
	}
	if (close(my_pipes->infile_fd) < 0)
		ft_printf(2, "%s\n", ERR_CLOSE);
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

/*Just double check the if statement here, there was a reason why you put it like that*/
void	reset_properties(t_pipes *my_pipes)
{
	if (ft_strcmp(my_pipes->command_path, my_pipes->command_node->cmd[0]))
	{
		free (my_pipes->command_path);
		my_pipes->command_path = NULL;
	}
	my_pipes->command_node = NULL;
	my_pipes->infile_fd = -1;
	my_pipes->outfile_fd = -1;
	my_pipes->heredoc_node = NULL;
	my_pipes->exit_status = 0;
	if (my_pipes->current_section != (my_pipes->pipe_amount + 1))
	{
		my_pipes->read_end = my_pipes->write_end - 1;
		if (my_pipes->current_section < my_pipes->pipe_amount)
			my_pipes->write_end = my_pipes->write_end + 2;
	//	printf("Moving to next pipe: read_end = %d, write_end = %d\n", my_pipes->read_end, my_pipes->write_end);
	//	printf("Curr section: %d\n", my_pipes->current_section);
	}
	my_pipes->current_section++;
}

void	close_child_pipes(t_pipes *my_pipes)
{
	int	i;

	i = 0;
	while (i < my_pipes->pipe_amount * 2)
	{
		if (my_pipes->pipes[i] != -1)
		{
			if (close(my_pipes->pipes[i]) < 0)
				ft_printf(2, "%s\n", ERR_CLOSE);
		}
		i++;
	}
	if (close(my_pipes->stdinfd) < 0)
		ft_printf(2, "%s\n", ERR_CLOSE);
	my_pipes->stdinfd = -1;
	if (close(my_pipes->stdoutfd) < 0)
		ft_printf(2, "%s\n", ERR_CLOSE);
	my_pipes->stdoutfd = -1;
}

/*Heredoc handling should not be here, it should be done in loop nodes*/
void	handle_redirections(t_node *node, t_pipes *my_pipes, int status)
{
	if (my_pipes->outfile_fd >= 0)
		redirection_outfile(my_pipes);
	if (my_pipes->infile_fd >= 0)
		redirection_infile(my_pipes);
	if (my_pipes->heredoc_node)
		heredoc(node, my_pipes, *my_pipes->my_envp, my_pipes->paths, status);
	if (my_pipes->current_section != 1 && my_pipes->infile_fd == -1)
	{
		if (dup2(my_pipes->pipes[my_pipes->read_end], STDIN_FILENO) < 0)
		{
			ft_printf(2, "%s\n", ERR_FD);
			my_pipes->exit_status = 1;
		}
	}
	if ((my_pipes->current_section != my_pipes->pipe_amount + 1) && my_pipes->outfile_fd == -1)
	{
		if (dup2(my_pipes->pipes[my_pipes->write_end], STDOUT_FILENO) < 0)
		{
			ft_printf(2, "%s\n", ERR_FD);
			my_pipes->exit_status = 1;
		}
	}
}

/*Function to close pipes in the parent.
- We close always the write end except for the last section because that end was already closed
- We close read always after we reach the second section, not in the first section because we still need it
- Then in the end we reset properties as well as go over to the next pair or pipe ends.

CLEAN THIS UP
*/
void	close_pipes(t_pipes *my_pipes)
{
	if (my_pipes->pipe_amount > 0)
	{
		if (my_pipes->current_section == 1)
		{
			if (close(my_pipes->pipes[my_pipes->write_end]) < 0)
				ft_printf(2, "%s\n", ERR_CLOSE);
			my_pipes->pipes[my_pipes->write_end] = -1;
		//	printf("Closing write end: %d\n", my_pipes->write_end);
		//	printf("Curr section: %d\n", my_pipes->current_section);
		}
		else if (my_pipes->current_section <= my_pipes->pipe_amount)
		{
			if (close(my_pipes->pipes[my_pipes->write_end]) < 0)
				ft_printf(2, "%s\n", ERR_CLOSE);
			my_pipes->pipes[my_pipes->write_end] = -1;
		//	printf("Closing write end: %d\n", my_pipes->write_end);
		//	printf("Curr section: %d\n", my_pipes->current_section);
		}
		if (my_pipes->current_section != (my_pipes->pipe_amount + 1))
		{
			if (my_pipes->current_section > 1)
			{
				if (close(my_pipes->pipes[my_pipes->read_end]) < 0)
					ft_printf(2, "%s\n", ERR_CLOSE);
				my_pipes->pipes[my_pipes->read_end] = -1;
		//		printf("Closing middle read end: %d\n", my_pipes->read_end);
			}
		}
		else
		{
			if (close(my_pipes->pipes[my_pipes->read_end]) < 0)
				ft_printf(2, "%s\n", ERR_CLOSE);
			my_pipes->pipes[my_pipes->read_end] = -1;
		//	printf("Closing last read end: %d\n", my_pipes->read_end);
		//	printf("Curr section: %d\n", my_pipes->current_section);
		}
	}
/*The reason we are doing this is so we can handle the exit if waitpid fails!!!
If this stays, you can clean up the reset properties a bit*/
	if (my_pipes->current_section != (my_pipes->pipe_amount + 1))
		reset_properties(my_pipes);
}

void	run_builtin_command(t_node *node, t_pipes *my_pipes)
{
	if (!ft_strcmp(node->cmd[0], "echo"))
		execute_echo(node, my_pipes->my_envp);
	else if (!ft_strcmp(node->cmd[0], "pwd"))
		execute_pwd();
	else if (!ft_strcmp(node->cmd[0], "env"))
		execute_env(my_pipes->my_envp);
	else if (!ft_strcmp(node->cmd[0], "export"))
		execute_export(node->cmd, my_pipes->my_envp);
	else if (!ft_strcmp(node->cmd[0], "unset"))
		execute_unset(node->cmd, my_pipes->my_envp);
	else if (!ft_strcmp(node->cmd[0], "exit"))
		execute_exit(node->cmd, my_pipes);
	else if (!ft_strcmp(node->cmd[0], "cd"))
		execute_cd(node->cmd);
}

/* Worth double checking if these commands fail*/
int	execute_builtin(t_node *node, t_pipes *my_pipes, int status)
{
	int	pid;

	if (my_pipes->pipe_amount > 0)
	{
		pid = fork();
		if (pid < 0)
			handle_fatal_exit(ERR_FORK, my_pipes, NULL, NULL);
		if (pid == 0)
		{
			handle_redirections(node, my_pipes, status);
			close_child_pipes(my_pipes);
			if (my_pipes->exit_status == 1)
				exit (1);
			run_builtin_command(node, my_pipes);
			exit(0);
		}
		else
			return (pid);
	}
	else
	{
		handle_redirections(node, my_pipes, status);
		if (my_pipes->exit_status == 1)
			return (0);
		run_builtin_command(node, my_pipes);
		return (0);
	}
}

/* EISDIR AND ENOEXEC behace unexpectedly.
Also it might be worthwhile to add these errno checks in a separate function.
*/
int	execute_executable(t_node *node, t_pipes *my_pipes, int status)
{
	int	pid;

	my_pipes->command_path = get_absolute_path(my_pipes->paths, node->cmd[0]);
	if (my_pipes->command_path == NULL)
		handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
	pid = fork();
	if (pid < 0)
		handle_fatal_exit(ERR_FORK, my_pipes, NULL, NULL);
	if (pid == 0)
	{
		handle_redirections(node, my_pipes, status);
		close_child_pipes(my_pipes);
		if (my_pipes->exit_status == 1)
			exit(1);
		execve(my_pipes->command_path, &node->cmd[0], *(my_pipes->my_envp));
		if (errno == ENOENT)
		{
			my_pipes->exit_status = 127;
			handle_fatal_exit(ERR_COMMAND, my_pipes, NULL, node->cmd[0]);
		}
/*This one is not working right now. Is there any use of function called stat?*/
		if (errno == EISDIR)
		{
			my_pipes->exit_status = 126;
			handle_fatal_exit(ERR_DIR, my_pipes, NULL, node->cmd[0]);
		}
		else if (errno == EACCES)
		{
			my_pipes->exit_status = 127;
			handle_fatal_exit(ERR_INVFILE, my_pipes, NULL, node->cmd[0]);
		}
		else if (errno == ENOEXEC)
		{
/* This one is still a bit mystery, it feels like in bash this doesn't behave in standard way*/
		}
		else
		{
			my_pipes->exit_status = 1;
			handle_fatal_exit(ERR_EXECVE, my_pipes, NULL, node->cmd[0]);
		}
	}
	return (pid);
}

int	is_builtin(char *command)
{
	const char	*builtins[7];

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	int i = 0;
	while (i <= 6)
	{
		if (!ft_strcmp(builtins[i], command))
			return (1);
		i++;
	}
	return (0);
}

//MAKE THIS SHORTER
void	initialize_struct(t_pipes *my_pipes, t_node *list, char ***envp)
{
	int	i;

	my_pipes->pipes = NULL;
	my_pipes->command_node = NULL;
	my_pipes->command_path = NULL;
	my_pipes->heredoc_node = NULL;
	my_pipes->paths = NULL;
	my_pipes->my_envp = envp;
	my_pipes->read_end = 0;
	my_pipes->write_end = 1;
	my_pipes->stdoutfd = -1;
	my_pipes->stdinfd = -1;
	my_pipes->infile_fd = -1;
	my_pipes->outfile_fd = -1;
	my_pipes->current_section = 1;
	my_pipes->pipe_amount = get_pipe_amount(list);
	my_pipes->exit_status = 0;
	if (my_pipes->pipe_amount > 0)
	{
		my_pipes->pipes = malloc(sizeof(int) * (my_pipes->pipe_amount * 2));
		if (my_pipes->pipes == NULL)
			handle_fatal_exit(MALLOC, my_pipes, list, NULL);
		i = 0;
		while (i < my_pipes->pipe_amount)
		{
			if (pipe(&my_pipes->pipes[i * 2]) < 0)
				handle_fatal_exit(ERR_PIPE, my_pipes, list, NULL);
			i++;
		}
	}
	my_pipes->paths = get_paths(my_pipes->my_envp);
	if (my_pipes->paths == NULL)
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
int	get_exit_status(pid_t child_pids[], int amount, t_pipes *my_pipes)
{
	int	status;
	int	exit_status;
	int	i;

	i = 0;
	status = 0;
	while (i < amount)
	{
		if (child_pids[i] > 0)
		{
			if (waitpid(child_pids[i], &status, 0) < 0)
				handle_fatal_exit(ERR_WAITPID, my_pipes, NULL, NULL);
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_status = 128 + WTERMSIG(status);
			else
				exit_status = my_pipes->exit_status;
		}
		else
			exit_status = my_pipes->exit_status;
		i++;
	}
	ft_printf(2, "Exit status is %d\n", exit_status);
//Perhaps these need another exit message
	if (dup2(my_pipes->stdinfd, STDIN_FILENO) < 0)
		handle_fatal_exit(ERR_FD, my_pipes, NULL, NULL);
	if (dup2(my_pipes->stdoutfd, STDOUT_FILENO) < 0)
		handle_fatal_exit(ERR_FD, my_pipes, NULL, NULL);
	free_my_pipes(my_pipes);
	return (exit_status);
}

//THIS ONE NEEDS TO BE CLEANED UP
//WORK ON HEREDOC, THATS BROKEN
//Also maybe chid pids could be added to my_pipes struct
//Shorten this
int	loop_nodes(t_node *list, char ***envp, int status)
{
	t_pipes	*my_pipes;
	int		i = 0;

	my_pipes = malloc(sizeof(t_pipes));
	if (my_pipes == NULL)
	{
		free_array(*envp);
		handle_fatal_exit(MALLOC, my_pipes, list, NULL);
	}
	initialize_struct(my_pipes, list, envp);
	pid_t child_pids[my_pipes->pipe_amount + 1];
	while (list != NULL)
	{
		if (list->type == COMMAND)
			my_pipes->command_node = list;
		if (list->type == REDIR_APPEND || list->type == REDIR_OUTF)
			set_outfile(list->file, list->type, my_pipes);
		if (list->type == REDIR_INF)
			open_infile(list->file, my_pipes);
		if (list->type == REDIR_HEREDOC)
			my_pipes->heredoc_node = list;
		if ((list->next == NULL) || (list->next && my_pipes->pipe_amount > 0 && list->next->type == PIPE))
		{
			if (my_pipes->command_node != NULL && is_builtin(my_pipes->command_node->cmd[0]) == 1)
			{
				child_pids[i] = execute_builtin(my_pipes->command_node, my_pipes, status);
				i++;
			}
			else if (my_pipes->command_node != NULL)
			{
				child_pids[i] = execute_executable(my_pipes->command_node, my_pipes, status);
				i++;
			}
			close_pipes(my_pipes);
		}
		list = list->next;
	}
	return (get_exit_status(child_pids, i, my_pipes));
}
