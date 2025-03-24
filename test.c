/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:26:20 by khiidenh          #+#    #+#             */
/*   Updated: 2025/03/24 12:17:43 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//Function that handles the actual execution
//Double check the waitpid here
static void execute_command(int single_command, t_node *node, char *envp[], t_pipes *my_pipes)
{
	char **paths = NULL;
	int		i = 0;
	ft_putstr_fd("are we here in exec cmd\n", 2);
	if (paths == NULL)
		paths = get_paths(envp);
	my_pipes->command_path = get_absolute_path(paths,node->cmd[0]);
	pid_t	pid;

	if (single_command == 1)
	{
		pid = fork();
		ft_putnbr_fd(pid, 2);
		if (pid == 0)
		{
			ft_putendl_fd(my_pipes->command_path, 2);
			// while (node->cmd[i])
			// 	printf("%s ", node->cmd[i++]);
			// printf("\n");
			execve(my_pipes->command_path, node->cmd, envp);
			printf("Error with exexve!\n");
		}
		int status;
		waitpid(pid, &status, 0);
		dup2(my_pipes->stdinfd, STDIN_FILENO);
		dup2(my_pipes->stdoutfd, STDOUT_FILENO);
		close (my_pipes->stdinfd);
		close (my_pipes->stdoutfd);
		printf("parent has waited for child %d\n", pid);
	}
	else
	{
		ft_putendl_fd(my_pipes->command_path, 2);
		// while (node->cmd[i])
		// 	printf("%s ", node->cmd[i++]);
		// printf("\n");
		execve(my_pipes->command_path, node->cmd, envp);
		printf("Error with exexve!\n");
	}
	// else if (pid > 0)
	// {
		// printf("Freeing command path!\n");
		// if (my_pipes->command_path != NULL)
		// {
		// free (my_pipes->command_path);
		// my_pipes->command_path = NULL;
		// }
	// }
	// if (single_command == 1)
	// {
	// int status;
	// waitpid(pid, &status, 0);
	// dup2(my_pipes->stdinfd, STDIN_FILENO);
	// dup2(my_pipes->stdoutfd, STDOUT_FILENO);
	// close (my_pipes->stdinfd);
	// close (my_pipes->stdoutfd);
	// printf("parent has waited for child %d\n", pid);
	// }
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
static int	get_pipe_amount(t_node *list)
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
static void execute_pipe(t_node *node, char *envp[], t_pipes *my_pipes)
{
	int i = my_pipes->pipe_amount * 2;
	int	pid;
my_pipes->current_section++;
printf("we here in exec pipe with %s\n", node->cmd[0]);
if (my_pipes->infile)
	printf("we have infile %s\n", my_pipes->infile->file);
else
	printf("no infile\n");
if (my_pipes->outfile)
	printf("we have outfile %s\n", my_pipes->outfile->file);
else
	printf("no outfile\n");
pid = fork();
printf("new pid is %d in %s\n", pid, node->cmd[0]);
if (pid == 0)
{
	if (node->next != NULL)
	{
		close (my_pipes->pipes[my_pipes->read_end]);
		if (my_pipes->outfile == NULL)
		{
			printf("we dup stdout to pipe end %d\n", my_pipes->write_end);
			if (dup2(my_pipes->pipes[my_pipes->write_end], STDOUT_FILENO) == -1)
				printf("Error with dup!\n");
			close (my_pipes->pipes[my_pipes->write_end]);
		}
		else
			redirection_outfile(my_pipes->outfile, envp, my_pipes);
	}
	else
		printf("next node null\n");
	ft_putstr_fd("are we in child\n", 2);
	if (my_pipes->current_section == 1)
		execute_command(0, node, envp, my_pipes);
	// if (my_pipes->current_section != 1)
	// {
	if (my_pipes->infile == NULL && my_pipes->current_section != 1)
	{
		//close (my_pipes->pipes[my_pipes->write_end]);
		ft_putstr_fd("we dup stdin to pipe end ", 2);
		ft_putnbr_fd(my_pipes->read_end, 2);
		ft_putstr_fd("\n", 2);
		if (dup2(my_pipes->pipes[my_pipes->read_end], STDIN_FILENO) == -1)
			printf("Error with dup!\n");
		close (my_pipes->pipes[my_pipes->read_end]);
		printf("curr sect not 1\n");
		execute_command(0, node, envp, my_pipes);
	}
	if (my_pipes->infile->file)
	{
		ft_putstr_fd("are we in child anymore\n", 2);
		redirection_infile(my_pipes->infile, envp, my_pipes);
	}
	ft_putstr_fd("are we in child anymore\n", 2);
	execute_command(0, node, envp, my_pipes);
	// }
	// while (i-- > 0)
	// 	close(my_pipes->pipes[i]);
}
else if (pid > 0)
{
	if ((my_pipes->current_section % 2) == 0)
	{
		close(my_pipes->pipes[my_pipes->read_end]);
		close(my_pipes->pipes[my_pipes->write_end]);
		//my_pipes->read_end = my_pipes->read_end + 2;
		//my_pipes->write_end = my_pipes->write_end + 2;
	}
	else
		close(my_pipes->pipes[my_pipes->write_end]);
	int status;
	waitpid(pid, &status, 0);
	printf("in exec pipe parent has waited for child %d %s\n", pid, node->cmd[0]);
	if (my_pipes->infile)
		my_pipes->infile = NULL;
	if (my_pipes->outfile)
		my_pipes->outfile = NULL;
}
}

//Initializing a struct to track stuffsies
static void	initialize_struct(t_pipes *my_pipes, t_node *list)
{
	int	i;

	my_pipes->infile = NULL;
	my_pipes->outfile = NULL;
	my_pipes->command_node = NULL;
	my_pipes->read_end = 0;
	my_pipes->write_end = 1;
	my_pipes->stdinfd = -1;
	my_pipes->stdoutfd = -1;
	my_pipes->current_section = 0;
	my_pipes->command_path = NULL;
	my_pipes->pipe_amount = get_pipe_amount(list);
	my_pipes->pipes = malloc(sizeof(int) * (my_pipes->pipe_amount * 2));
	i = 0;
	while (i < my_pipes->pipe_amount * 2)
	{
		pipe(&my_pipes->pipes[i]);
		i = i + 2;
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
	// my_pipes->outfile = NULL;
	// my_pipes->infile = NULL;
	curr = curr->next;
	}
	// stuff that needs to be freed
// 	if (my_pipes != NULL) {
// 	if (my_pipes->command_path != NULL)
// 	{
// 		free(my_pipes->command_path);
// 		my_pipes->command_path = NULL;
// 	}
// 	if (my_pipes->pipes != NULL)
// 	{
// 		 int i = 0;
// 		 while (i < my_pipes->pipe_amount * 2)
// 		 {
// 		 	close(my_pipes->pipes[i]);
// 		 	i++;
// 		 }
// 		free(my_pipes->pipes);
// 		my_pipes->pipes = NULL;
// 	}
// 	free(my_pipes);
// }
}
