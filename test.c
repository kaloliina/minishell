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
		if (my_pipes->command_path && ft_strcmp(my_pipes->command_path, my_pipes->command_node->cmd[0]))
		{
			free(my_pipes->command_path);
			my_pipes->command_path = NULL;
		}
		if (close (my_pipes->stdinfd) < 0)
			ft_printf(2, "%s\n", ERR_CLOSE);
		if (close (my_pipes->stdoutfd) < 0)
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

//Malloc, Fork, Waitpid...
static void	handle_fatal_exit(char *msg, t_pipes *my_pipes, t_node *list)
{
	if (list == NULL)
		free_nodes(my_pipes->command_node);
	else
		free_nodes(list);
	if (my_pipes != NULL)
	{
		free_array(*my_pipes->my_envp);
		free_my_pipes(my_pipes);
	}
	ft_printf(2, "%s\n", msg);
	exit (1);
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

void	reset_properties(t_pipes *my_pipes)
{
	if (ft_strcmp(my_pipes->command_path, my_pipes->command_node->cmd[0]))
		free (my_pipes->command_path);
	my_pipes->command_path = NULL;
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
    if (close(my_pipes->stdoutfd) < 0)
		ft_printf(2, "%s\n", ERR_CLOSE);
}
//ADD PROPER ERROR FOR DUP
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
//The reason we are doing this is so we can handle the exit if waitpid fails!!!
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

//DOUBLE CHECK THESE COMMANDS FAILING
//MOVE THE STD RESTORATION TO THE VERY END
int	execute_builtin(t_node *node, t_pipes *my_pipes, int status)
{
	int	pid;

	if (my_pipes->pipe_amount > 0)
	{
		pid = fork();
		if (pid < 0)
			handle_fatal_exit(ERR_FORK, my_pipes, NULL);
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
			return (1);
		run_builtin_command(node, my_pipes);
		return (0);
	}
}

//DOUBLE CHECK THIS ALSO EACCESS
int	execute_executable(t_node *node, t_pipes *my_pipes, int status)
{
	int	pid;

	my_pipes->command_path = get_absolute_path(my_pipes->paths, node->cmd[0]);
	if (my_pipes->command_path == NULL)
		handle_fatal_exit(MALLOC, my_pipes, NULL);
	pid = fork();
	if (pid < 0)
		handle_fatal_exit(ERR_FORK, my_pipes, NULL);
	if (pid == 0)
	{
		handle_redirections(node, my_pipes, status);
		close_child_pipes(my_pipes);
		if (my_pipes->exit_status == 1)
			exit(1);
		execve(my_pipes->command_path, &node->cmd[0], *(my_pipes->my_envp));
		if (errno == ENOENT)
		{
			ft_printf(2, "%s: %s\n", node->cmd[0], ERR_COMMAND);
			free (my_pipes->command_path);
			my_pipes->command_path = NULL;
			exit (127);
		}
		else if (errno == EACCES)
		{
			ft_printf(2, "%s: no permissions\n", node->cmd[0]);
			free (my_pipes->command_path);
			my_pipes->command_path = NULL;
			exit (126);
		}
		else
			exit(1);
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
			handle_fatal_exit(MALLOC, my_pipes, list);
		i = 0;
		while (i < my_pipes->pipe_amount)
		{
			if (pipe(&my_pipes->pipes[i * 2]) < 0)
				handle_fatal_exit(ERR_PIPE, my_pipes, list);
			i++;
		}
	}
	my_pipes->paths = get_paths(my_pipes->my_envp);
	if (my_pipes->paths == NULL)
		handle_fatal_exit(MALLOC, my_pipes, list);
	my_pipes->stdoutfd = dup(STDOUT_FILENO);
	if (my_pipes->stdoutfd == -1)
		handle_fatal_exit(ERR_FD, my_pipes, list);
	my_pipes->stdinfd = dup(STDIN_FILENO);
	if (my_pipes->stdinfd == -1)
		handle_fatal_exit(ERR_FD, my_pipes, list);
}

//DOUBLE CHECK EXIT STATUSES
//Also there might be a situation with sleep where you explicitly have to mark the last process but I was unable to repro the behaviour
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
				handle_fatal_exit(ERR_WAITPID, my_pipes, NULL);
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_status = 128 + WTERMSIG(status);
			else
				exit_status = my_pipes->exit_status;
				//printf("parent exited with status of %d\n", my_pipes->exit_status);
		}
		i++;
	}
	//these maybe would need another exit message
	if (dup2(my_pipes->stdinfd, STDIN_FILENO) < 0)
		handle_fatal_exit(ERR_FD, my_pipes, NULL);
	if (dup2(my_pipes->stdoutfd, STDOUT_FILENO) < 0)
		handle_fatal_exit(ERR_FD, my_pipes, NULL);
	free_my_pipes(my_pipes);
	return (exit_status);
}

//THIS ONE NEEDS TO BE CLEANED UP
int	loop_nodes(t_node *list, char ***envp, int status)
{
	t_pipes	*my_pipes;
	int		i = 0;

	my_pipes = malloc(sizeof(t_pipes));
	if (my_pipes == NULL)
	{
		free_array(*envp);
		handle_fatal_exit(MALLOC, my_pipes, list);
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
/*
- Clean up the My_pipes struct
- Rename the struct
- Add child pids int array into the struct(?)
- Clean up the initialize struct section
*/
