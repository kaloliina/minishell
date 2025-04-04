#include "../minishell.h"

//Needs dup checks
//Function to handle outfile redirection
static void	redirection_outfile(t_pipes *my_pipes)
{
	if (my_pipes->stdoutfd == -1)
		my_pipes->stdoutfd = dup(STDOUT_FILENO);
	dup2(my_pipes->outfile_fd, STDOUT_FILENO);
	close(my_pipes->outfile_fd);
}

//Needs dup checks
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
		printf("Moving to next pipe: read_end = %d, write_end = %d\n", my_pipes->read_end, my_pipes->write_end);
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
		dup2(my_pipes->pipes[my_pipes->read_end], STDIN_FILENO);
	if ((my_pipes->current_section != my_pipes->pipe_amount + 1) && my_pipes->outfile_fd == -1)
		dup2(my_pipes->pipes[my_pipes->write_end], STDOUT_FILENO);
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
			printf("Closing write end: %d\n", my_pipes->write_end);
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
//Check the return value here, I assume it's okay to return 0 when parent
int	execute_builtin(t_node *node, t_pipes *my_pipes)
{
	int	pid;
	my_pipes->current_section++;
	if (my_pipes->pipe_amount > 0)
	{
		pid = fork();
		if (pid == 0)
		{
			handle_redirections(node, my_pipes);
			run_builtin_command(node, my_pipes);
			exit(0);
		}
		else
		{
			close_pipes(node, my_pipes);
			return (pid);
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

	my_pipes->current_section++;
	pid = fork();
	if (pid == 0)
	{
		if (my_pipes->paths == NULL)
			my_pipes->paths = get_paths(my_pipes->my_envp);
		handle_redirections(node, my_pipes);
		my_pipes->command_path = get_absolute_path(my_pipes->paths, node->cmd[0]);
		printf("absolute: %s\n", my_pipes->command_path);
		if (my_pipes->exit_status == 1)
			exit(1);
		execve(my_pipes->command_path, &node->cmd[0], my_pipes->my_envp);
//rename the printf thingies
		if (errno == ENOENT)
		{
			ft_printf(2, "%s: command not found\n", node->cmd[0]);
			exit (127);
		}
		else if (errno == EACCES)
		{
			ft_printf(2, "%s: no permissions\n", &node->cmd[0]);
			exit (126);
		}
		else
			exit(1);
	}
	close_pipes(node, my_pipes);
	return (pid);
}
//Wonder if these could be saved in the header
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

//This functions needs to be divided
//I also need to handle the exit status here in a neater way
//If theres a pipeline and it contains of exexutables, we should be able to get the exit status here
//If there's a pipeline and it contains both, we need to know which one was the last?
//If theres one command which is executable, we can get the exit status
//If there's one command which is builtiin, we need to get the exit status via struct..
//Feels simpler if I would just purely update the statuses in a struct, but I will need to know
//Also maybe we should be returning the exit status instead of the myenvp
char	**loop_nodes(t_node *list, char *envp[])
{
	t_node	*curr;
	t_pipes	*my_pipes;
	char	**return_envp;
	int		status;
	int		i = 0;

	curr = list;
	my_pipes = malloc(sizeof(t_pipes));
	//error check here
	initialize_struct(my_pipes, list, envp);
	pid_t child_pids[my_pipes->pipe_amount + 1];
	while (curr != NULL)
	{
		if (curr->type == COMMAND)
			my_pipes->command_node = curr;
		if (curr->type == REDIR_APPEND || curr->type == REDIR_OUTF)
			set_outfile(curr->file, curr->type, my_pipes);
		if (curr->type == REDIR_INF)
			open_infile(curr->file, my_pipes);
		if (curr->type == REDIR_HEREDOC)
			my_pipes->heredoc_node = curr;
		if ((curr->next == NULL) || (curr->next && my_pipes->pipe_amount > 0 && curr->next->type == PIPE))
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
		}
		curr = curr->next;
	}
	return_envp = my_pipes->my_envp;
	int j = 0;
	while (j < i)
	{
		printf("pidsies %d\n", child_pids[j]);
		if (child_pids[j] > 0)
			waitpid(child_pids[j], &status, 0);
		j++;
	}
	free_my_pipes(my_pipes);
//	printf("pids %d\n", status);
//this is not correct.. instead we should be checking if something is not forked vs forked.
//why is this not catching the status.. with utils.c | grep int i get something went wrong instead the status..?
	if (WIFEXITED(status) == true)
		printf("child exited with status of %d\n", WEXITSTATUS(status));
	else
		printf("something went wrong");

		printf("exit status is %d", my_pipes->exit_status);
	return (return_envp);
}
//echo hi > testi.txt fails
//also echo hi > test.txt gets permission denied
// cat < utils.c | grep int should work but if it's just <utils |  grep int, we get a segfalt
