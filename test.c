/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:26:20 by khiidenh          #+#    #+#             */
/*   Updated: 2025/03/26 12:24:23 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//Function that handles the actual execution
//Double check the waitpid here
void	execute_command(int single_command, t_node *node, char *envp[], t_pipes *my_pipes)
{
	int	pid;
	int	status;

	if (my_pipes->paths == NULL)
		my_pipes->paths = get_paths(envp);
	my_pipes->command_path = get_absolute_path(my_pipes->paths, node->cmd[0]);
	if (single_command == 1)
		pid = fork();
	else
		pid = 0;
	if (pid == 0)
	{
		execve(my_pipes->command_path, &node->cmd[0], envp);
		write(2, "Error", 5);
	}
	else if (pid > 0)
	{
		if (single_command == 1)
			waitpid(pid, &status, 0);
	}
}

//Function to handle outfile redirection
static void	redirection_outfile(t_node *node, char *envp[], t_pipes *my_pipes)
{
	int	fd;

	if (node->type == REDIR_OUTF)
		fd = set_outfile(node->file, 0);
	if (node->type == REDIR_APPEND)
		fd = set_outfile(node->file, 1);
	if (my_pipes->stdoutfd == -1)
		my_pipes->stdoutfd = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

//Function to handle infile redirection
static void	redirection_infile(t_node *node, char *envp[], t_pipes *my_pipes)
{
	int	fd;

	fd = open_infile(node->file);
	if (my_pipes->stdinfd == -1)
		my_pipes->stdinfd = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
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
	my_pipes->outfile = NULL;
	my_pipes->infile = NULL;
	free (my_pipes->command_path);
	my_pipes->command_path = NULL;
	my_pipes->command_node = NULL;
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
void	execute_pipe(t_node *node, char *envp[], t_pipes *my_pipes)
{
	int	i;
	int	pid;
	int	status;

	i = 0;
	my_pipes->current_section++;
	pid = fork();
	if (pid == 0)
	{
		if (my_pipes->outfile != NULL)
			redirection_outfile(my_pipes->outfile, envp, my_pipes);
		if (my_pipes->infile != NULL)
			redirection_infile(my_pipes->infile, envp, my_pipes);
		if (my_pipes->current_section != 1 && my_pipes->infile == NULL)
			dup2(my_pipes->pipes[my_pipes->read_end], STDIN_FILENO);
		if (node->next != NULL && my_pipes->outfile == NULL)
			dup2(my_pipes->pipes[my_pipes->write_end], STDOUT_FILENO);
		while (i < my_pipes->pipe_amount * 2)
		{
			if (i != my_pipes->read_end && i != my_pipes->write_end)
				close(my_pipes->pipes[i]);
			i++;
		}
		execute_command(0, node, envp, my_pipes);
	}
	else
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
		waitpid(pid, &status, 0);
	}
}

//Initializing a struct to track stuffsies
void	initialize_struct(t_pipes *my_pipes, t_node *list)
{
	int	i;

	my_pipes->pipes = NULL;
	my_pipes->infile = NULL;
	my_pipes->outfile = NULL;
	my_pipes->command_node = NULL;
	my_pipes->command_path = NULL;
	my_pipes->paths = NULL;
	my_pipes->read_end = 0;
	my_pipes->write_end = 1;
	my_pipes->stdoutfd = -1;
	my_pipes->stdinfd = -1;
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

	curr = list;
	my_pipes = malloc(sizeof(t_pipes));
	initialize_struct(my_pipes, list);
	while (curr != NULL)
	{
		if (curr->type == COMMAND)
			my_pipes->command_node = curr;
		if (curr->type == REDIR_APPEND || curr->type == REDIR_OUTF)
		{
			if (my_pipes->pipe_amount == 0)
				redirection_outfile(curr, envp, my_pipes);
			my_pipes->outfile = curr;
		}
		if (curr->type == REDIR_INF)
		{
			if (my_pipes->pipe_amount == 0)
				redirection_infile(curr, envp, my_pipes);
			my_pipes->infile = curr;
		}
		if (curr->next != NULL && my_pipes->pipe_amount > 0 && curr->next->type == PIPE)
			execute_pipe(my_pipes->command_node, envp, my_pipes);
		if (my_pipes->pipe_amount > 0 && curr->next == NULL)
			execute_pipe(my_pipes->command_node, envp, my_pipes);
		if (curr->next == NULL && my_pipes->pipe_amount == 0)
			execute_command(1, my_pipes->command_node, envp, my_pipes);
		curr = curr->next;
	}
	free_my_pipes(my_pipes);
}
