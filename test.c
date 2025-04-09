#include "../minishell.h"

//Needs proper error trigger
static void	redirection_outfile(t_pipes *my_pipes)
{
	if (my_pipes->stdoutfd == -1)
	{
		my_pipes->stdoutfd = dup(STDOUT_FILENO);
		if (my_pipes->stdoutfd == -1)
		{
			ft_printf(2, "%s\n", ERR_FD);
			my_pipes->exit_status = 1;
		}
	}
	if (dup2(my_pipes->outfile_fd, STDOUT_FILENO) < 0)
	{
		ft_printf(2, "%s\n", ERR_FD);
		my_pipes->exit_status = 1;
	}
	if (close(my_pipes->outfile_fd) < 0)
	{
		ft_printf(2, "%s\n", ERR_CLOSE);
		my_pipes->exit_status = 1;
	}
}

//Needs proper error trigger
static void	redirection_infile(t_pipes *my_pipes)
{
	if (my_pipes->stdinfd == -1)
	{
		my_pipes->stdinfd = dup(STDIN_FILENO);
		if (my_pipes->stdinfd == -1)
		{
			ft_printf(2, "%s\n", ERR_FD);
			my_pipes->exit_status = 1;
		}
	}
	if (dup2(my_pipes->infile_fd, STDIN_FILENO) < 0)
	{
		ft_printf(2, "%s\n", ERR_FD);
		my_pipes->exit_status = 1;
	}
	if (close(my_pipes->infile_fd) < 0)
	{
		ft_printf(2, "%s\n", ERR_CLOSE);
		my_pipes->exit_status = 1;
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

void	reset_properties(t_pipes *my_pipes)
{
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
		printf("Moving to next pipe: read_end = %d, write_end = %d\n", my_pipes->read_end, my_pipes->write_end);
		printf("Curr section: %d\n", my_pipes->current_section);
	}
	my_pipes->current_section++;
}

void	handle_redirections(t_node *node, t_pipes *my_pipes)
{
	int	i;

	i = 0;
	if (my_pipes->outfile_fd >= 0)
		redirection_outfile(my_pipes);
	if (my_pipes->infile_fd >= 0)
		redirection_infile(my_pipes);
	if (my_pipes->heredoc_node)
		heredoc(node, my_pipes, *my_pipes->my_envp, my_pipes->paths);
	if (my_pipes->current_section != 1 && my_pipes->infile_fd == -1)
	{
		if (dup2(my_pipes->pipes[my_pipes->read_end], STDIN_FILENO) < 0)
		{
			//error
		}
	}
	if ((my_pipes->current_section != my_pipes->pipe_amount + 1) && my_pipes->outfile_fd == -1)
	{
		if (dup2(my_pipes->pipes[my_pipes->write_end], STDOUT_FILENO) < 0)
		{
			//error
		}
	}
	while (i < my_pipes->pipe_amount * 2)
	{
		if (close(my_pipes->pipes[i]) < 0)
		{
			//error
		}
		i++;
	}
}

/*Function to close pipes in the parent.
- We close always the write end except for the last section because that end was already closed
- We close read always after we reach the second section, not in the first section because we still need it
- Then in the end we reset properties as well as go over to the next pair or pipe ends.

Needs proper error trigger
*/
void	close_pipes(t_pipes *my_pipes)
{
	if (my_pipes->pipe_amount > 0)
	{
		if (my_pipes->current_section == 1)
		{
			if (close(my_pipes->pipes[my_pipes->write_end]) < 0)
			{
				//error
			}
			printf("Closing write end: %d\n", my_pipes->write_end);
			printf("Curr section: %d\n", my_pipes->current_section);
		}
		else if (my_pipes->current_section <= my_pipes->pipe_amount)
		{
			if (close(my_pipes->pipes[my_pipes->write_end]) < 0)
			{
				//error
			}
			printf("Closing write end: %d\n", my_pipes->write_end);
			printf("Curr section: %d\n", my_pipes->current_section);
		}
		if (my_pipes->current_section != (my_pipes->pipe_amount + 1))
		{
			if (my_pipes->current_section > 1)
			{
				if (close(my_pipes->pipes[my_pipes->read_end]) < 0)
				{
					//error
				}
				printf("Closing middle read end: %d\n", my_pipes->read_end);
			}
		}
		else
		{
			if (close(my_pipes->pipes[my_pipes->read_end]) < 0)
			{
				//error
			}
			printf("Closing last read end: %d\n", my_pipes->read_end);
			printf("Curr section: %d\n", my_pipes->current_section);
		}
	}
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
//Check the return value here, I assume it's okay to return 0 when parent
//Also can these commands fail as well
int	execute_builtin(t_node *node, t_pipes *my_pipes)
{
	int	pid;

	if (my_pipes->pipe_amount > 0)
	{
		pid = fork();
		if (pid < 0)
		{
			ft_printf(2, "%s\n", ERR_FORK);
			exit (1);
		}
		if (pid == 0)
		{
			if (my_pipes->exit_status == 1)
				exit (1);
			handle_redirections(node, my_pipes);
			run_builtin_command(node, my_pipes);
			exit(0);
		}
		else
			return (pid);
	}
	else
	{
		if (my_pipes->exit_status == 1)
			return (0);
		handle_redirections(node, my_pipes);
		run_builtin_command(node, my_pipes);
//Im not even sure if this should be done here. Basically yes but this is also a "last thing to do"
		if (my_pipes->stdinfd != -1)
		{
			if (dup2(my_pipes->stdinfd, STDIN_FILENO) < 0)
			{
				//error
			}
			if (close (my_pipes->stdinfd) < 0)
			{
				//error
			}
		}
		if (my_pipes->stdoutfd != -1)
		{
			if (dup2(my_pipes->stdoutfd, STDOUT_FILENO) < 0)
			{
				//error
			}
			if (close (my_pipes->stdoutfd) < 0)
			{
				//error
			}
		}
		return (0);
	}
}
//Question is what should we track here, should we catch the wrong commands etc here
//Or should we catch them already in the get_paths..?
//Also right now if there's a problem with fd's, I still go here and I return from this stage
//The reason is that this is in child so it's easier for me to get the exit status.
int	execute_executable(t_node *node, t_pipes *my_pipes)
{
	int	pid;

	pid = fork();
	if (pid < 0)
	{
		free_array(*my_pipes->my_envp);
		free_my_pipes(my_pipes);
		ft_printf(2, "%s\n", ERR_FORK);
		exit (1);
	}
	if (pid == 0)
	{
		if (my_pipes->paths == NULL)
			my_pipes->paths = get_paths(my_pipes->my_envp);
		handle_redirections(node, my_pipes);
		my_pipes->command_path = get_absolute_path(my_pipes->paths, node->cmd[0]);
		printf("absolute: %s\n", my_pipes->command_path);
		if (my_pipes->exit_status == 1)
			exit(1);
		execve(my_pipes->command_path, &node->cmd[0], *(my_pipes->my_envp));
		if (errno == ENOENT)
		{
			ft_printf(2, "%s: %s\n", node->cmd[0], ERR_COMMAND);
			exit (127);
		}
//double check this
		else if (errno == EACCES)
		{
			ft_printf(2, "%s: no permissions\n", &node->cmd[0]);
			exit (126);
		}
		else
			exit(1);
	}
	return (pid);
}
//Question is what should we track here, should we catch the wrong commands etc here
//Or should we catch them already in the get_paths..?
//Also right now if there's a problem with fd's, I still go here and I return from this stage
//The reason is that this is in child so it's easier for me to get the exit status.
int	execute_executable(t_node *node, t_pipes *my_pipes)
{
	int	pid;

//Also right now if we write exit XX, it exits with 0 when in reality it should exit with the number told
//We also need to implement exit function
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

//Make this shorter
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
		{
			ft_printf(2, "%s\n", MALLOC);
			exit (1);
		}
		i = 0;
		while (i < my_pipes->pipe_amount)
		{
			if (pipe(&my_pipes->pipes[i * 2]) < 0)
			{
				ft_printf(2, "%s\n", ERR_PIPE);
				exit (1);
			}
			i++;
		}
	}
}

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
		if (my_pipes->pipes)
		{
//I actually doubt we even need to close pipes here because I'm closing them step by step!!
//However I might need to do some sort of closing here just in case if closing or fork fails..
			// while (i < my_pipes->pipe_amount * 2)
			// {
			// 	if (close(my_pipes->pipes[i]) < 0)
			// 	{
			// 		//close failed
			// 	}
			// 	i++;
			// }
			free(my_pipes->pipes);
			my_pipes->pipes = NULL;
		}
		free(my_pipes);
	}
}

//Double check the exit statuses, they seem to work but test with edgecases!
//Also there might be a situation with sleep where you explicitly have to mark the last process but I was unable to repro the behaviour
int	get_exit_status(pid_t child_pids[], int amount, t_pipes *my_pipes)
{
	int	status;
	int	exit_status;
	int	i;

	i = 0;
	while (i < amount)
	{
		if (child_pids[i] > 0)
		{
			if (waitpid(child_pids[i], &status, 0) < 0)
			{
				ft_printf(2, "%s\n", ERR_WAITPID);
				exit (1);
			}
		}
		i++;
	}
	free_my_pipes(my_pipes);
	if (WIFEXITED(status) == true)
	{
		exit_status = WEXITSTATUS(status);
		printf("child exited with status of %d\n", WEXITSTATUS(status));
	}
	else
	{
		exit_status = my_pipes->exit_status;
		printf("parent exited with status of %d\n", my_pipes->exit_status);
	}
	return (exit_status);
}

int	loop_nodes(t_node *list, char ***envp)
{
	t_pipes	*my_pipes;
	int		i = 0;

	my_pipes = malloc(sizeof(t_pipes));
	if (my_pipes == NULL)
	{
		ft_printf(2, "%s\n", MALLOC);
		exit (1);
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
				child_pids[i] = execute_builtin(my_pipes->command_node, my_pipes);
				i++;
			}
			else if (my_pipes->command_node != NULL)
			{
				child_pids[i] = execute_executable(my_pipes->command_node, my_pipes);
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
