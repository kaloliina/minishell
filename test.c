/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:26:20 by khiidenh          #+#    #+#             */
/*   Updated: 2025/03/25 15:21:37 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//Function that handles the actual execution
//Double check the waitpid here
void execute_command(int single_command, t_node *node, char *envp[])
{
	char **paths = NULL;
	if (paths == NULL)
	{
	paths = get_paths(envp);
	}
	char *path = NULL;
	if (path != NULL)
		free (path);
	path = get_absolute_path(paths,node->cmd[0]);
	int	pid;
	if (single_command == 1)
	{
		pid = fork();
	}
	else
		pid = 0;
	ft_putstr_fd(path, 2);
	ft_putstr_fd(node->cmd[0], 2);
	ft_putstr_fd(node->cmd[1], 2);
	ft_putstr_fd("\n", 2);
	if (pid == 0)
	{
		execve(path, &node->cmd[0], envp);
		write(2, "Errer", 5);
	}
	else if (pid > 0)
	{
	if (single_command == 1)
	{
		int status;
		waitpid(pid, &status, 0);
	}
	free (path);
	int i = 0;
	if (paths != NULL)
	{
		while (paths[i] != NULL)
		{
		free(paths[i]);
		i++;
		}
		free (paths);
	}
	}
}

//Function to handle outfile redirection
static void redirection_outfile(t_node *node, char *envp[], t_pipes *my_pipes)
{
	int fd;
	int	backup;
	if (node->type == REDIR_OUTF)
		fd = set_outfile(node->file, 0);
	if (node->type == REDIR_APPEND)
		fd = set_outfile(node->file, 1);
	backup = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	my_pipes->stdoutfd = backup;
}

//Function to handle infile redirection
static void redirection_infile(t_node *node, char *envp[], t_pipes *my_pipes)
{
	int fd;
	int	backup;
	fd = open_infile(node->file);
	backup = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	my_pipes->stdinfd = backup;
}

//Helper function to get pipe amount
int	get_pipe_amount(t_node *list)
{
	t_node *curr;
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


//This function handles the actual pipe execution
void execute_pipe(t_node *node, char *envp[], t_pipes *my_pipes)
{
	int i = my_pipes->pipe_amount * 2;
	int	pid;
	my_pipes->current_section++;
	pid = fork();
if (pid == 0)
{
//this was the latest change you did.. double check
if (node->next != NULL || my_pipes->outfile != NULL)
{
	if (my_pipes->outfile == NULL)
	{
		ft_putnbr_fd(my_pipes->write_end, 2);
		ft_putstr_fd("first\n", 2);
		dup2(my_pipes->pipes[my_pipes->write_end], STDOUT_FILENO);
	}
	else
		redirection_outfile(my_pipes->outfile, envp, my_pipes);
}
if (my_pipes->current_section != 1)
{
	if (my_pipes->infile == NULL)
	{
		ft_putnbr_fd(my_pipes->read_end, 2);
		ft_putstr_fd("second\n", 2);
		ft_putstr_fd("are we here\n", 2);
		dup2(my_pipes->pipes[my_pipes->read_end], STDIN_FILENO);
	}
	else
	{
		ft_putstr_fd("are wii here\n", 2);
		redirection_infile(my_pipes->infile, envp, my_pipes);
	}
}
	for (int j = 0; j < my_pipes->pipe_amount * 2; j++)
	{
	if (j != my_pipes->read_end && j != my_pipes->write_end)
		close(my_pipes->pipes[j]);
	}
	execute_command(0, node, envp);
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
	int status;
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
	my_pipes->read_end = 0;
	my_pipes->write_end = 1;
	my_pipes->stdinfd = -1;
	my_pipes->stdoutfd = -1;
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

//This function goes over the entire list of nodes, handling redirections and pipes
//Heredoc yet remains to be handled...
void	loop_nodes(t_node *list, char *envp[])
{
	t_node	*curr;
	t_pipes *my_pipes;

	curr = list;
	my_pipes = malloc(sizeof(t_pipes));
	initialize_struct(my_pipes, list);
	while (curr != NULL)
	{
	if (curr->type == COMMAND)
		my_pipes->command_node = curr;
	if (curr->type == REDIR_APPEND || curr->type == REDIR_OUTF)
	{
		printf("did we find redir\n");
		if (my_pipes->pipe_amount == 0)
			redirection_outfile(curr, envp, my_pipes);
		my_pipes->outfile = curr;
	}
	if (curr->type == REDIR_INF)
	{
		printf("did we find redir_inf\n");
		if (my_pipes->pipe_amount == 0)
			redirection_infile(curr, envp, my_pipes);
		my_pipes->infile = curr;
	}
	if (curr->next != NULL && my_pipes->pipe_amount > 0 && curr->next->type == PIPE)
		execute_pipe(my_pipes->command_node, envp, my_pipes);
	if (my_pipes->pipe_amount > 0 && curr->next == NULL)
		execute_pipe(my_pipes->command_node, envp, my_pipes);
	if (curr->next == NULL && my_pipes->pipe_amount == 0)
		execute_command(1, my_pipes->command_node, envp);
//we cant put these to null yet, put them elsewhere?
	// my_pipes->outfile = NULL;
	// my_pipes->infile = NULL;
	my_pipes->command_node = NULL;
	curr = curr->next;
	}
	// what if we wait here instead of we wait with mod
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
		if (my_pipes->command_path != NULL)
		{
			free(my_pipes->command_path);
			my_pipes->command_path = NULL;
		}
		if (my_pipes->pipes != NULL)
		{
			int i = 0;
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



