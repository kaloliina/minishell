/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:26:20 by khiidenh          #+#    #+#             */
/*   Updated: 2025/03/20 17:07:35 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../libft/includes/libft.h"
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef enum s_type
{
	PIPE,
	COMMAND,
	REDIR_INF,
	REDIR_OUTF,
	REDIR_APPEND,
	REDIR_HEREDOC,
}	t_type;

typedef struct s_node
{
	t_type			type;
	char			*file;
	char			**args;
	char			*cmd;
	struct s_node	*prev;
	struct s_node	*next;
}					t_node;

int	set_outfile(char *file, int append);
int	open_infile(char *file);
char	**get_paths(char *envp[]);
char	*get_absolute_path(char **paths, char *command);

//This is a helper function to combine the command and args together
char	**combine_commands(t_node *node)
{
int i = 0;
char **commands;
if (node->args != NULL)
{
while (node->args[i] != NULL)
{
	i++;
}
}
commands = malloc(sizeof(char *) * (i + 2));
commands[i + 1] = NULL;
commands[0] = strdup(node->cmd);
int j = 0;
while (j < i)
{
	commands[j + 1] = strdup(node->args[j]);
	j++;
}
return (commands);
}

//Function that handles the actual execution
//Double check the waitpid here
void execute_command(int single_command, t_node *node, char *envp[])
{
	char **paths = get_paths(envp);
	char *path = get_absolute_path(paths,node->cmd);
	char **commands;
	commands = combine_commands(node);
	int	pid;
	if (single_command == 1)
	{
		pid = fork();
	}
	else
		pid = 0;
	if (pid == 0)
	{
		execve(path, commands, envp);
	}
	if (single_command == 1)
	{
		int status;
		waitpid(pid, &status, 0);
	}
}

//Function to handle outfile redirection
void redirection_outfile(t_node *node, char *envp[])
{
	int fd;
	if (node->type == REDIR_OUTF)
		fd = set_outfile(node->file, 0);
	if (node->type == REDIR_APPEND)
		fd = set_outfile(node->file, 1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

//Function to handle infile redirection
void redirection_infile(t_node *node, char *envp[])
{
	int fd;
	fd = open_infile(node->file);
	dup2(fd, STDIN_FILENO);
	close(fd);
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

//This struct holds all the relevant data regarding fd's, number of pipes, what's the section's command node etc.
typedef struct t_pipe
{
	int	*pipes;
	struct s_node *infile;
	struct s_node *outfile;
	int	current_section;
	int	pipe_amount;
	struct s_node *command_node;
	int	read_end;
	int	write_end;
}					t_pipes;


//This function handles the actual pipe execution
void execute_pipe(t_node *node, char *envp[], t_pipes *my_pipes)
{
	int i = my_pipes->pipe_amount * 2;
	int	pid;
my_pipes->current_section++;
pid = fork();
if (pid == 0)
{
if (node->next != NULL)
{
	if (my_pipes->outfile == NULL)
	{
		dup2(my_pipes->pipes[my_pipes->write_end], STDOUT_FILENO);
	}
	else
		redirection_outfile(my_pipes->outfile, envp);
}
if (my_pipes->current_section != 1)
{
	if (my_pipes->infile == NULL)
	{
		dup2(my_pipes->pipes[my_pipes->read_end], STDIN_FILENO);
	}
	else
		redirection_infile(my_pipes->infile, envp);
}
	while (i-- > 0)
		close(my_pipes->pipes[i]);
	execute_command(0, node, envp);
}
else
{
	close(my_pipes->pipes[my_pipes->write_end]);
	if (my_pipes->current_section > 1)
		close(my_pipes->pipes[my_pipes->read_end]);
	my_pipes->read_end = my_pipes->read_end + 2;
	my_pipes->write_end = my_pipes->write_end + 2;
	int status;
	waitpid(pid, &status, 0);
}
}

//Initializing a struct to track stuffsies
void	initialize_struct(t_pipes *my_pipes, t_node *list)
{
	int	i;

	my_pipes->infile = NULL;
	my_pipes->outfile = NULL;
	my_pipes->command_node = NULL;
	my_pipes->read_end = -2;
	my_pipes->write_end = 1;
	my_pipes->current_section = 0;
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
			redirection_outfile(curr, envp);
		my_pipes->outfile = curr;
	}
	if (curr->type == REDIR_INF)
	{
		if (my_pipes->pipe_amount == 0)
			redirection_infile(curr, envp);
		my_pipes->infile = curr;
	}
	if (curr->next != NULL && my_pipes->pipe_amount > 0 && curr->next->type == PIPE)
		execute_pipe(my_pipes->command_node, envp, my_pipes);
	if (my_pipes->pipe_amount > 0 && curr->next == NULL)
		execute_pipe(my_pipes->command_node, envp, my_pipes);
	if (curr->next == NULL && my_pipes->pipe_amount == 0)
		execute_command(1, my_pipes->command_node, envp);
	my_pipes->outfile = NULL;
	my_pipes->infile = NULL;
	curr = curr->next;
	}
}

//SAME HERE THIS IS NOT WORKING
// int main(int argc, char *argv[], char *envp[])
// {
// 	t_node *node0;
// 	node0 = malloc(sizeof(t_node));
// 	node0->type = COMMAND;
// 	node0->file = NULL;
// 	node0->cmd = "ls";
// 	node0->args = NULL;

// //next node
// 	t_node *node1;
// 	node1 = malloc(sizeof(t_node));
// 	node1->type = PIPE;
// 	node1->file = NULL;
// 	node1->cmd = NULL;
// 	node1->args = NULL;

// 	t_node *node2;
// 	node2 = malloc(sizeof(t_node));
// 	node2->type = COMMAND;
// 	node2->file = NULL;
// 	node2->cmd = "grep";
// 	node2->args = malloc(sizeof(char *) * 2);
// 	node2->args[0] = ".c";
// 	node2->args[1] = NULL;

// 	t_node *node3;
// 	node3 = malloc(sizeof(t_node));
// 	node3->type = PIPE;
// 	node3->file = NULL;
// 	node3->cmd = NULL;
// 	node3->args = NULL;

// 	t_node *node4;
// 	node4 = malloc(sizeof(t_node));
// 	node4->type = COMMAND;
// 	node4->file = NULL;
// 	node4->cmd = "grep";
// 	node4->args = malloc(sizeof(char *) * 2);
// 	node4->args[0] = "test";
// 	node4->args[1] = NULL;

// 	t_node *node5;
// 	node5 = malloc(sizeof(t_node));
// 	node5->type = PIPE;
// 	node5->file = NULL;
// 	node5->cmd = NULL;
// 	node5->args = NULL;

// 	t_node *node6;
// 	node6 = malloc(sizeof(t_node));
// 	node6->type = COMMAND;
// 	node6->file = NULL;
// 	node6->cmd = "wc";
// 	node6->args = malloc(sizeof(char *) * 2);
// 	node6->args[0] = "-l";
// 	node6->args[1] = NULL;


// 	node0->next = node1;
// 	node0->prev  = NULL;
// 	node1->next = node2;
// 	node1->prev = node0;
// 	node2->next = node3;
// 	node2->prev = node1;
// 	node3->next = node4;
// 	node3->prev = node2;
// 	node4->next = node5;
// 	node4->prev = node3;
// 	node5->next = node6;
// 	node5->prev = node4;
// 	node6->next = NULL;
// 	node6->prev = node5;

// 	loop_nodes(node0, envp);
// 	return (0);

// }


// int main(int argc, char *argv[], char *envp[])
// {
// 	t_node *node0;
// 	node0 = malloc(sizeof(t_node));
// 	node0->type = COMMAND;
// 	node0->file = NULL;
// 	node0->cmd = "sort";
// 	node0->args = NULL;

// 	t_node *node1;
// 	node1 = malloc(sizeof(t_node));
// 	node1->type = REDIR_INF;
// 	node1->file = "outfile.txt";
// 	node1->cmd = NULL;
// 	node1->args = NULL;

// 	// t_node *node2;
// 	// node2 = malloc(sizeof(t_node));
// 	// node2->type = PIPE;
// 	// node2->file = NULL;
// 	// node2->cmd = NULL;
// 	// node2->args = NULL;

// 	// t_node *node3;
// 	// node3 = malloc(sizeof(t_node));
// 	// node3->type = COMMAND;
// 	// node3->file = NULL;
// 	// node3->cmd = "wc";
// 	// node3->args = malloc(sizeof(char *) * 2);
// 	// node3->args[0] = "-l";
// 	// node3->args[1] = NULL;

// 	node0->next = node1;
// 	node0->prev  = NULL;
// 	node1->next = NULL;
// 	node1->prev = node0;
// 	// node2->next = node3;
// 	// node2->prev = node1;
// 	// node3->next = NULL;
// 	// node3->prev = node2;


// 	loop_nodes(node0, envp);
// 	return (0);

// }

//THIS IS SHOULD GIVE ERROR IF THERE IS NO FILE..
// int main(int argc, char *argv[], char *envp[])
// {
// 	t_node *node0;
// 	node0 = malloc(sizeof(t_node));
// 	node0->type = COMMAND;
// 	node0->file = NULL;
// 	node0->cmd = "cat";
// 	node0->args = NULL;

// 	t_node *node1;
// 	node1 = malloc(sizeof(t_node));
// 	node1->type = REDIR_OUTF;
// 	node1->file = "a";
// 	node1->cmd = NULL;
// 	node1->args = NULL;

// 	t_node *node2;
// 	node2 = malloc(sizeof(t_node));
// 	node2->type = REDIR_OUTF;
// 	node2->file = "b";
// 	node2->cmd = NULL;
// 	node2->args = NULL;

// 	t_node *node3;
// 	node3 = malloc(sizeof(t_node));
// 	node3->type = REDIR_INF;
// 	node3->file = "c";
// 	node3->cmd = NULL;
// 	node3->args = NULL;

// 	node0->next = node1;
// 	node0->prev  = NULL;
// 	node1->next = node2;
// 	node1->prev = node0;
// 	node2->next = node3;
// 	node2->prev = node1;
// 	node3->next = NULL;
// 	node3->prev = node2;


// 	loop_nodes(node0, envp);
// 	return (0);
//  }

// int main(int argc, char *argv[], char *envp[])
// {
// 	t_node *node0;
// 	node0 = malloc(sizeof(t_node));
// 	node0->type = COMMAND;
// 	node0->file = NULL;
// 	node0->cmd = "echo";
// 	node0->args = malloc(sizeof(char *) * 2);
// 	node0->args[0] = "hi there!";
// 	node0->args[1] = NULL;

// 	t_node *node1;
// 	node1 = malloc(sizeof(t_node));
// 	node1->type = REDIR_APPEND;
// 	node1->file = "outfile.txt";
// 	node1->cmd = NULL;
// 	node1->args = NULL;

// 	t_node *node2;
// 	node2 = malloc(sizeof(t_node));
// 	node2->type = PIPE;
// 	node2->file = NULL;
// 	node2->cmd = NULL;
// 	node2->args = NULL;

// 	t_node *node3;
// 	node3 = malloc(sizeof(t_node));
// 	node3->type = COMMAND;
// 	node3->file = NULL;
// 	node3->cmd = "wc";
// 	node3->args = malloc(sizeof(char *) * 2);
// 	node3->args[0] = "-l";
// 	node3->args[1] = NULL;

// 	t_node *node4;
// 	node4 = malloc(sizeof(t_node));
// 	node4->type = REDIR_INF;
// 	node4->file = "outfile.txt";
// 	node4->cmd = NULL;
// 	node4->args = NULL;

// 	node0->next = node1;
// 	node0->prev  = NULL;
// 	node1->next = node2;
// 	node1->prev = node0;
// 	node2->next = node3;
// 	node2->prev = node1;
// 	node3->next = node4;
// 	node3->prev = node2;
// 	node4->next = NULL;
// 	node4->prev = node3;


// 	loop_nodes(node0, envp);
// 	return (0);

// }


//THIS IS NOT WORKING!!!
int main(int argc, char *argv[], char *envp[])
{
	t_node *node0;
	node0 = malloc(sizeof(t_node));
	node0->type = COMMAND;
	node0->file = NULL;
	node0->cmd = "ls";
	node0->args = NULL;

//next node
	t_node *node1;
	node1 = malloc(sizeof(t_node));
	node1->type = PIPE;
	node1->file = NULL;
	node1->cmd = NULL;
	node1->args = NULL;

	t_node *node2;
	node2 = malloc(sizeof(t_node));
	node2->type = COMMAND;
	node2->file = NULL;
	node2->cmd = "grep";
	node2->args = malloc(sizeof(char *) * 2);
	node2->args[0] = ".c";
	node2->args[1] = NULL;

	t_node *node3;
	node3 = malloc(sizeof(t_node));
	node3->type = PIPE;
	node3->file = NULL;
	node3->cmd = NULL;
	node3->args = NULL;

	t_node *node4;
	node4 = malloc(sizeof(t_node));
	node4->type = COMMAND;
	node4->file = NULL;
	node4->cmd = "wc";
	node4->args = malloc(sizeof(char *) * 2);
	node4->args[0] = "-l";
	node4->args[1] = NULL;


	node0->next = node1;
	node0->prev  = NULL;
	node1->next = node2;
	node1->prev = node0;
	node2->next = node3;
	node2->prev = node1;
	node3->next = node4;
	node3->prev = node2;
	node4->next = NULL;
	node4->prev = node3;


	loop_nodes(node0, envp);
	return (0);

}

// int main(int argc, char *argv[], char *envp[])
// {
// 	t_node *node0;
// 	node0 = malloc(sizeof(t_node));
// 	node0->type = COMMAND;
// 	node0->file = NULL;
// 	node0->cmd = "cat";
// 	node0->args = malloc(sizeof(char *) * 2);
// 	node0->args[0] = "newfile.txt";
// 	node0->args[1] = NULL;

// //next node
// 	t_node *node1;
// 	node1 = malloc(sizeof(t_node));
// 	node1->type = PIPE;
// 	node1->file = NULL;
// 	node1->cmd = NULL;
// 	node1->args = NULL;

// 	t_node *node2;
// 	node2 = malloc(sizeof(t_node));
// 	node2->type = COMMAND;
// 	node2->file = NULL;
// 	node2->cmd = "wc";
// 	node2->args = malloc(sizeof(char *) * 2);
// 	node2->args[0] = "-l";
// 	node2->args[1] = NULL;

// 	node0->next = node1;
// 	node0->prev  = NULL;
// 	node1->next = node2;
// 	node1->prev = node0;
// 	node2->next = NULL;
// 	node2->prev = node1;

// 	loop_nodes(node0, envp);
// 	return (0);

// }


// int main(int argc, char *argv[], char *envp[])
// {
// 	t_node *node0;
// 	node0 = malloc(sizeof(t_node));
// 	node0->type = COMMAND;
// 	node0->file = NULL;
// 	node0->cmd = "ls";
// 	node0->args = malloc(sizeof(char *) * 2);
// 	node0->args[0] = "-l";
// 	node0->args[1] = NULL;

// //next node
// 	t_node *node1;
// 	node1 = malloc(sizeof(t_node));
// 	node1->type = PIPE;
// 	node1->file = NULL;
// 	node1->cmd = NULL;
// 	node1->args = NULL;

// 	t_node *node2;
// 	node2 = malloc(sizeof(t_node));
// 	node2->type = COMMAND;
// 	node2->file = NULL;
// 	node2->cmd = "grep";
// 	node2->args = malloc(sizeof(char *) * 2);
// 	node2->args[0] = ".txt";
// 	node2->args[1] = NULL;

// 	node0->next = node1;
// 	node0->prev  = NULL;
// 	node1->next = node2;
// 	node1->prev = node0;
// 	node2->next = NULL;
// 	node2->prev = node1;

// 	loop_nodes(node0, envp);
// 	return (0);

// }


//if we have ls for example, the file should be part of the commands, but wc which expects standardinput, then the file should be separately.
//	char *path = "/usr/bin/echo";

// int main(int argc, char *argv[], char *envp[])
// {
// //	printf("%s", paths[0]);
// 	t_node *node0;
// 	node0 = malloc(sizeof(t_node));
// 	node0->type = COMMAND;
// 	node0->file = NULL;
// 	node0->cmd = "echo";
// 	node0->args = malloc(sizeof(char *) * 2);
// 	node0->args[0] = "Hehe hello hi there";
// 	node0->args[1] = NULL;

// //next node
// 	t_node *node1;
// 	node1 = malloc(sizeof(t_node));
// 	node1->type = REDIR_OUTF;
// 	node1->file = "newfile.txt";
// 	node1->cmd = NULL;
// 	node1->args = NULL;
// 	node1->next = NULL;

// //	node0->next = node1;
// //	node1->next = NULL;

// 	node0->next = NULL;

// 	loop_nodes(node0, envp);

// 	return (0);

// }

// int main(int argc, char *argv[], char *envp[])
// {
// 	char **paths = get_paths(envp);
// //	printf("%s", paths[0]);
// 	t_node *node0;
// 	node0 = malloc(sizeof(t_node));
// 	node0->type = COMMAND;
// 	node0->file = NULL;
// 	node0->cmd = "echo";
// 	node0->args = malloc(sizeof(char *) * 2);
// 	node0->args[0] = "Hehe hello hi there";
// 	node0->args[1] = NULL;
// 	char *absolute_path = get_absolute_path(paths, node0->cmd);

// //next node
// 	t_node *node1;
// 	node1 = malloc(sizeof(t_node));
// 	node1->type = REDIR_APPEND;
// 	node1->file = "newfile.txt";
// 	node1->cmd = NULL;
// 	node1->args = NULL;
// 	node1->next = NULL;

// 	node0->next = node1;
// 	node1->next = NULL;

// //	node0->next = NULL;

// 	loop_nodes(node0, absolute_path, envp);

// 	return (0);

// }
