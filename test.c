/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:26:20 by khiidenh          #+#    #+#             */
/*   Updated: 2025/03/31 14:47:40 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	execute_echo(t_node *node, char **envp)
{
	int		i;
	char	*temp;

	i = 1;
	while (node->cmd[i])
	{

		printf("%s", node->cmd[i]);
		if (node->cmd[i + 1])
			printf(" ");
		i++;
	}
	printf("\n");
}

void	execute_pwd()
{
	char	*buf;

	buf = malloc(100);
	if (!buf)
	{
		printf("minishell: memory allocation failure\n");
		exit (1);
	}
	getcwd(buf, 100);
	if (!buf)
		perror("minishell:");
	else
		printf("%s\n", buf);
	free (buf);
}


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
	while (curr != NULL)
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
}

//This function handles the actual pipe execution
/*
If:
First we check if it has outfile or infile, if yes, then we redirect stdout to outfile or redirect stdin to read from infile.
Then we check if next is not null, so instead of the output being redirected to stdout, we direct it to the pipe's write end.
This won't happen in the last section because then we want the output to go to stdout!
After this instead of reading from standard input, we read from pipe's read end.
This won't happen in the first section because we want to read from standard input.

Else:
The first if statement closes the previous write end.
The second moves to the next pipe.
The last one closes the last read end.
This one still has issues!*/
void	close_pipes(t_node *node, t_pipes *my_pipes)
{
	if (my_pipes->pipe_amount > 0)
	{
		if (my_pipes->current_section > 1)
		{
			close(my_pipes->pipes[my_pipes->write_end - 2]);
			printf("Closing previous write end: %d\n", my_pipes->write_end - 2);
		}
		//Next pipies
		if (node->next != NULL)
		{
			my_pipes->read_end = my_pipes->write_end - 1;
			my_pipes->write_end = my_pipes->write_end + 2;
			printf("Moving to next pipeeez: read_end = %d, write_end = %d\n", my_pipes->read_end, my_pipes->write_end);
		}
		else
		{
			close(my_pipes->pipes[my_pipes->read_end]);
			printf("Closing last read end: %d\n", my_pipes->read_end);
		}
		reset_properties(my_pipes);
	}
}


void execute_builtin(t_node *node, t_pipes *my_pipes, char *envp[])
{
	my_pipes->current_section++;
	if (my_pipes->outfile_fd >= 0)
		redirection_outfile(my_pipes);
	if (my_pipes->infile_fd >= 0)
		redirection_infile(my_pipes);
	if (my_pipes->current_section != 1 && my_pipes->infile_fd == -1)
		dup2(my_pipes->pipes[my_pipes->read_end], STDIN_FILENO);
	if (node->next != NULL && my_pipes->outfile_fd == -1)
		dup2(my_pipes->pipes[my_pipes->write_end], STDOUT_FILENO);
	if (ft_strcmp(node->cmd[0], "echo") == 0)
		execute_echo(node, envp);
	if (ft_strcmp(node->cmd[0], "pwd") == 0)
		execute_pwd();
	close_pipes(node, my_pipes);
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

int execute_executable(t_node *node, char *envp[], t_pipes *my_pipes)
{
	int	i;
	int	pid;
	int	status;

	i = 0;
	my_pipes->current_section++;
	pid = fork();
	if (pid == 0)
	{
		if (my_pipes->outfile_fd >= 0)
			redirection_outfile(my_pipes);
		if (my_pipes->infile_fd >= 0)
			redirection_infile(my_pipes);
		if (my_pipes->current_section != 1 && my_pipes->infile_fd == -1)
		{
			dup2(my_pipes->pipes[my_pipes->read_end], STDIN_FILENO);
			close(my_pipes->pipes[my_pipes->read_end]);
		}
		if (node->next != NULL && my_pipes->outfile_fd == -1)
		{
		//mby overthinking this but im actually sceptical do we get here always when we want to get here because we are sending the command node into
		//this function, which doesnt necessarily mean that next is NULL. Maybe instead i should have if current section is not pipeamount + 1.
			dup2(my_pipes->pipes[my_pipes->write_end], STDOUT_FILENO);
			close(my_pipes->pipes[my_pipes->write_end]);
		}
		while (i < my_pipes->pipe_amount * 2)
		{
			if (i != my_pipes->read_end && i != my_pipes->write_end)
				close(my_pipes->pipes[i]);
			i++;
		}
		if (my_pipes->paths == NULL)
			my_pipes->paths = get_paths(envp);
		my_pipes->command_path = get_absolute_path(my_pipes->paths, node->cmd[0]);
		execve(my_pipes->command_path, &node->cmd[0], envp);
		write(2, "Error", 5);
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
		if (ft_strcmp(builtins[i], command) == 0)
			return (1);
		i++;
	}
	return (0);
}


//Initializing a struct to track stuffsies
//I dont even know if I need the pipe_amount, look at what other things you may not need
void	initialize_struct(t_pipes *my_pipes, t_node *list)
{
	int	i;

	my_pipes->pipes = NULL;
	my_pipes->command_node = NULL;
	my_pipes->command_path = NULL;
	my_pipes->paths = NULL;
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
	if (my_pipes != NULL)
	{
		if (my_pipes->paths != NULL)
		{
			while (my_pipes->paths[i] != NULL)
			{
				free(my_pipes->paths[i]);
				i++;
			}
			free (my_pipes->paths);
		}
		i = 0;
		if (my_pipes->command_path != NULL)
		{
			free(my_pipes->command_path);
			my_pipes->command_path = NULL;
		}
		if (my_pipes->pipes != NULL)
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
void	loop_nodes(t_node *list, char *envp[])
{
	t_node	*curr;
	t_pipes	*my_pipes;

	int status;
	int i = 0;

	curr = list;
	my_pipes = malloc(sizeof(t_pipes));
	initialize_struct(my_pipes, list);
	pid_t child_pids[my_pipes->pipe_amount + 1];
	while (curr != NULL)
	{
		if (curr->type == COMMAND)
			my_pipes->command_node = curr;
		if (curr->type == REDIR_APPEND || curr->type == REDIR_OUTF)
			my_pipes->outfile_fd = set_outfile(curr->file, curr->type);
		if (curr->type == REDIR_INF)
			my_pipes->infile_fd = open_infile(curr->file);
		if ((curr->next == NULL) || (curr->next != NULL && my_pipes->pipe_amount > 0 && curr->next->type == PIPE))
		{
			if (is_builtin(my_pipes->command_node->cmd[0]) == 1)
				execute_builtin(my_pipes->command_node, my_pipes, envp);
			else
			{
				child_pids[i] = execute_executable(my_pipes->command_node, envp, my_pipes);
				i++;
			}
		}
		curr = curr->next;
	}
	int j = 0;
	while (j < i)
	{
			waitpid(child_pids[j], &status, 0);
			j++;
	}
	free_my_pipes(my_pipes);
}
