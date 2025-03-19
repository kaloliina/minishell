/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:26:20 by khiidenh          #+#    #+#             */
/*   Updated: 2025/03/19 12:32:36 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../libft/includes/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
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
void execute_command(int pid, t_node *node, char *envp[])
{
	char **paths = get_paths(envp);
	char *path = get_absolute_path(paths,node->cmd);
	char **commands;
	commands = combine_commands(node);
	if (pid == -1)
		pid = fork();

	if (pid == 0)
	{
		execve(path, commands, envp);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}

//Function to handle outfile redirection
void redirection_outfile(t_node *node, char *envp[])
{
	int fd;
	if (node->next->type == REDIR_OUTF)
		fd = set_outfile(node->next->file, 0);
	if (node->next->type == REDIR_APPEND)
		fd = set_outfile(node->next->file, 1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

//Function to handle infile redirection
void redirection_infile(t_node *node, char *envp[])
{
	int fd;
	fd = open_infile(node->next->file);
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

//This is used basically to only track the ends of the pipes, feels a bit unnecessary to have a struct with only one component
typedef struct t_pipe
{
	int	*pipes;
}					t_pipes;

//This one needs cleaning up
void execute_pipe(t_node *node, char *envp[])
{
	int	pipe_amount;
	pipe_amount = (get_pipe_amount(node)) * 2;
	t_pipes my_pipes;
	static int step = 0;
	static int i = 0;
	int	pid;
	static int counter;
	static int j = -2;
	static int k = 1;
if (step == 0)
{
	my_pipes.pipes = malloc(sizeof(int) * pipe_amount);
	while (i < pipe_amount)
	{
		pipe(&my_pipes.pipes[i]);
		i = i + 2;
	}
	counter = (pipe_amount / 2) + 1;
}
step++;
pid = fork();
if (pid == 0)
{
if (step != counter)
{
	if (node->next != NULL && node->next->type == REDIR_OUTF ||  node->next != NULL && node->next->type == REDIR_APPEND ||  node->next != NULL && node->next->type == REDIR_INF)
	{
		printf("here inoutf\n");
		redirection_outfile(node, envp);
	}
	else
	{
		dup2(my_pipes.pipes[k], STDOUT_FILENO);
	}
}
if (step != 1)
{
	if (node->next != NULL && node->next->type == REDIR_OUTF ||  node->next != NULL && node->next->type == REDIR_APPEND ||  node->next != NULL && node->next->type == REDIR_INF)
	{
		printf("here!!");
		redirection_infile(node, envp);
	}
	else
	{
		printf("hereeaa\n");
		dup2(my_pipes.pipes[j], STDIN_FILENO);
	}
}
	while (i-- > 0)
		close(my_pipes.pipes[i]);
	execute_command(pid, node, envp);
}
else
{
	if (step == counter)
	{
		while (i-- > 0)
			close(my_pipes.pipes[i]);
	}
	j = j + 2;
	k = k + 2;
}
}

//This function goes over the entire list of nodes, handling redirections and pipes
//Heredoc yet remains to be handled...
void	loop_nodes(t_node *list, char *envp[])
{
	t_node	*curr;
	t_node	*command_node;
	int		pipe_amount;

	curr = list;
	pipe_amount = get_pipe_amount(list);
	while (curr != NULL)
	{
	if (curr->type == COMMAND)
		command_node = curr;
	if (pipe_amount == 0)
	{
		if (curr->next != NULL && (curr->next->type == REDIR_APPEND || curr->next->type == REDIR_OUTF))
			redirection_outfile(curr, envp);
		if (curr->next != NULL && curr->next->type == REDIR_INF)
			redirection_infile(curr, envp);
	}
	if (curr->next != NULL && pipe_amount > 0 && curr->type != REDIR_APPEND && curr->type != REDIR_OUTF && curr->type != REDIR_INF && curr->type != PIPE)
		execute_pipe(curr, envp);
	if (curr->next == NULL && pipe_amount == 0)
		execute_command(-1, command_node, envp);
	curr = curr->next;
	}
}

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

//THIS IS NOT WORKING CURRENTLY
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

int main(int argc, char *argv[], char *envp[])
{
	t_node *node0;
	node0 = malloc(sizeof(t_node));
	node0->type = COMMAND;
	node0->file = NULL;
	node0->cmd = "echo";
	node0->args = malloc(sizeof(char *) * 2);
	node0->args[0] = "hi there!";
	node0->args[1] = NULL;

	t_node *node1;
	node1 = malloc(sizeof(t_node));
	node1->type = REDIR_APPEND;
	node1->file = "outfile.txt";
	node1->cmd = NULL;
	node1->args = NULL;

	t_node *node2;
	node2 = malloc(sizeof(t_node));
	node2->type = PIPE;
	node2->file = NULL;
	node2->cmd = NULL;
	node2->args = NULL;

	t_node *node3;
	node3 = malloc(sizeof(t_node));
	node3->type = COMMAND;
	node3->file = NULL;
	node3->cmd = "wc";
	node3->args = malloc(sizeof(char *) * 2);
	node3->args[0] = "-l";
	node3->args[1] = NULL;

	t_node *node4;
	node4 = malloc(sizeof(t_node));
	node4->type = REDIR_INF;
	node4->file = "outfile.txt";
	node4->cmd = NULL;
	node4->args = NULL;

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
// 	node4->cmd = "wc";
// 	node4->args = malloc(sizeof(char *) * 2);
// 	node4->args[0] = "-l";
// 	node4->args[1] = NULL;


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
