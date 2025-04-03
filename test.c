#include "../minishell.h"

//Function to handle outfile redirection
static void	redirection_outfile(t_pipes *my_pipes)
{
	if (my_pipes->stdoutfd == -1)
		my_pipes->stdoutfd = dup(STDOUT_FILENO);
	dup2(my_pipes->outfile_fd, STDOUT_FILENO);
	close(my_pipes->outfile_fd);
}

//Function to handle infile redirection
static void	redirection_infile(t_pipes *my_pipes)
{
	if (my_pipes->stdinfd == -1)
		my_pipes->stdinfd = dup(STDIN_FILENO);
	dup2(my_pipes->infile_fd, STDIN_FILENO);
	close(my_pipes->infile_fd);
}

//Helper function to get pipe amount
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
	if (my_pipes->current_section != (my_pipes->pipe_amount + 1))
	{
		my_pipes->read_end = my_pipes->write_end - 1;
		if (my_pipes->current_section < my_pipes->pipe_amount)
			my_pipes->write_end = my_pipes->write_end + 2;
		printf("Moving to next pipeeez: read_end = %d, write_end = %d\n", my_pipes->read_end, my_pipes->write_end);
		printf("Curr section: %d\n", my_pipes->current_section);
	}
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
		heredoc(node, my_pipes, my_pipes->my_envp, my_pipes->paths);
	if (my_pipes->current_section != 1 && my_pipes->infile_fd == -1)
	{
		dup2(my_pipes->pipes[my_pipes->read_end], STDIN_FILENO);
	}
	if ((my_pipes->current_section != my_pipes->pipe_amount + 1) && my_pipes->outfile_fd == -1)
	{
		dup2(my_pipes->pipes[my_pipes->write_end], STDOUT_FILENO);
	}	
	while (i < my_pipes->pipe_amount * 2)
	{
		close(my_pipes->pipes[i]);
		i++;
	}
}

/*Function to close pipes in the parent.
- We close always the write end except for the last section because that end was already closed
- We close read always after we reach the second section, not in the first section because we still need it
- Then in the end we reset properties as well as go over to the next pair or pipe ends. */
void	close_pipes(t_node *node, t_pipes *my_pipes)
{
	if (my_pipes->pipe_amount > 0)
	{
		if (my_pipes->current_section == 1)
		{
			close(my_pipes->pipes[my_pipes->write_end]);
			printf("Closing write endii %d\n", my_pipes->write_end);
			printf("Curr section: %d\n", my_pipes->current_section);
        }
		else if (my_pipes->current_section <= my_pipes->pipe_amount)
		{
			close(my_pipes->pipes[my_pipes->write_end]);
			printf("Closing write end: %d\n", my_pipes->write_end);
			printf("Curr section: %d\n", my_pipes->current_section);
		}
		if (node->next != NULL)
		{
			if (my_pipes->current_section > 1)
			{
				close(my_pipes->pipes[my_pipes->read_end]);
				printf("Closing middle read end: %d\n", my_pipes->read_end);
			}
		}
		else
		{
			close(my_pipes->pipes[my_pipes->read_end]);
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
		execute_export(node->cmd, &my_pipes->my_envp);
	else if (!ft_strcmp(node->cmd[0], "unset"))
		execute_unset(node->cmd, &my_pipes->my_envp);
	else if (!ft_strcmp(node->cmd[0], "cd"))
		execute_cd(node->cmd);
}

void	execute_builtin(t_node *node, t_pipes *my_pipes)
{
	my_pipes->current_section++;
	if (my_pipes->pipe_amount > 0)  
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			handle_redirections(node, my_pipes);
			run_builtin_command(node, my_pipes);
			exit(0);
		}
		else
		{
			close_pipes(node, my_pipes);
		}
	}
	else  
	{
		handle_redirections(node, my_pipes);
		run_builtin_command(node, my_pipes);
		if (my_pipes->stdinfd != -1)
		{
			dup2(my_pipes->stdinfd, STDIN_FILENO);
			close (my_pipes->stdinfd);
		}
		if (my_pipes->stdoutfd != -1)
		{
			dup2(my_pipes->stdoutfd, STDOUT_FILENO);
			close (my_pipes->stdoutfd);
		}
	}
}

int	execute_executable(t_node *node, t_pipes *my_pipes)
{
	int	pid;

	my_pipes->current_section++;
	pid = fork();
	if (pid == 0)
	{
		if (my_pipes->paths == NULL)
			my_pipes->paths = get_paths(my_pipes->my_envp);
		handle_redirections(node, my_pipes);
		my_pipes->command_path = get_absolute_path(my_pipes->paths, node->cmd[0]);
		execve(my_pipes->command_path, &node->cmd[0], my_pipes->my_envp);
		ft_printf(2, "minishell: execve failure\n");
	}
	close_pipes(node, my_pipes);
	return (pid);
}

int	is_builtin(char *command)
{
	const char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	int i = 0;
	while (i <= 6)
	{
		if (!ft_strcmp(builtins[i], command))
			return (1);
		i++;
	}
	return (0);
}

//Initializing a struct to track stuffsies
//I dont even know if I need the pipe_amount, look at what other things you may not need
void	initialize_struct(t_pipes *my_pipes, t_node *list, char **envp)
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
	my_pipes->current_section = 0;
	my_pipes->pipe_amount = get_pipe_amount(list);
	if (my_pipes->pipe_amount > 0)
	{
		my_pipes->pipes = malloc(sizeof(int) * (my_pipes->pipe_amount * 2));
		i = 0;
		while (i < my_pipes->pipe_amount)
		{
			pipe(&my_pipes->pipes[i * 2]);
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
			while (i < my_pipes->pipe_amount * 2)
			{
				close(my_pipes->pipes[i]);
				i++;
			}
			free(my_pipes->pipes);
			my_pipes->pipes = NULL;
		}
		free(my_pipes);
	}
}

//This function goes over the entire list of nodes, handling redirections and pipes
//Heredoc yet remains to be handled...
//We should wait for processes in loop nodes rather that in execute pipes and execute command!
char	**loop_nodes(t_node *list, char *envp[])
{
	t_node	*curr;
	t_pipes	*my_pipes;
	char	**return_envp;
	int status;
	int i = 0;

	curr = list;
	my_pipes = malloc(sizeof(t_pipes));
	initialize_struct(my_pipes, list, envp);
	pid_t child_pids[my_pipes->pipe_amount + 1];
	while (curr != NULL)
	{
		if (curr->type == COMMAND)
			my_pipes->command_node = curr;
		if (curr->type == REDIR_APPEND || curr->type == REDIR_OUTF)
			my_pipes->outfile_fd = set_outfile(curr->file, curr->type);
		if (curr->type == REDIR_INF)
			my_pipes->infile_fd = open_infile(curr->file);
		if (curr->type == REDIR_HEREDOC)
			my_pipes->heredoc_node = curr;
		if ((curr->next == NULL) || (curr->next && my_pipes->pipe_amount > 0 && curr->next->type == PIPE))
		{
			if (is_builtin(my_pipes->command_node->cmd[0]) == 1)
			{
				execute_builtin(my_pipes->command_node, my_pipes);
			}
			else
			{
				child_pids[i] = execute_executable(my_pipes->command_node, my_pipes);
				i++;
			}
		}
		curr = curr->next;
	}
	return_envp = my_pipes->my_envp;
	int j = 0;
	while (j < i)
	{
		if (child_pids[j] > 0)
			waitpid(child_pids[j], &status, 0);
		j++;
	}
	free_my_pipes(my_pipes);
	return (return_envp);
}
