/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:26:20 by khiidenh          #+#    #+#             */
/*   Updated: 2025/03/18 16:14:50 by khiidenh         ###   ########.fr       */
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

void execute_command(int pid, t_node *node, char *envp[])
{
	char **paths = get_paths(envp);
	char *path = get_absolute_path(paths,node->cmd);
	char **commands;
	commands = combine_commands(node);
	if (pid == -1)
	{
		pid = fork();
	}
	if (pid == 0)
	{
		execve(path, commands, envp);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}

void redirection_outfile(t_node *node, char *envp[])
{
	int pid;
	pid = fork();
	if (pid == 0)
	{
	int fd;

	if (node->next->type == REDIR_OUTF)
		fd = set_outfile(node->next->file, 0);
	if (node->next->type == REDIR_APPEND)
		fd = set_outfile(node->next->file, 1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	execute_command(pid, node, envp);
	}
}

void redirection_infile(t_node *node, char *envp[])
{
	int pid;
	pid = fork();
	if (pid == 0)
	{
	int fd;
	fd = open_infile(node->next->file);
	dup2(fd, STDIN_FILENO);
	close(fd);
	execute_command(pid, node, envp);
	}
}

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

typedef struct t_pipe
{
	int	*pipes;
}					t_pipes;


// void execute_pipe(t_node *node, char *envp[])
// {
// 	int	pipe_amount;
// 	pipe_amount = get_pipe_amount(node);
// 	pipe_amount = pipe_amount * 2;
// 	t_pipes my_pipes;
// 	static int status = 0;
// 	int i = 0;
// 	static int j = 0;
// 	static int k = 1;
// 	int	pid;
// if (status == 0)
// {
// 	status = 1;
// 	my_pipes.pipes = malloc(sizeof(int) * pipe_amount);
// 	while (i < pipe_amount)
// 	{
// 		pipe(&my_pipes.pipes[i]);
// 		i = i + 2;
// 	}
// }
// pipe_amount = pipe_amount / 2;
// pid = fork();

// if (pid == 0)
// {
// if (status == 1)
// {
// 	dup2(my_pipes.pipes[1], STDOUT_FILENO);
// 	k = k + 2;
// 	status++;
// }
// else if (status == 2)
// {
// 	dup2(my_pipes.pipes[0], STDIN_FILENO);
// 	dup2(my_pipes.pipes[3], STDOUT_FILENO);
// 	j = j + 2;
// 	status++;
// }
// else if (status == 3)
// {
// 	dup2(my_pipes.pipes[2], STDIN_FILENO);
// 	status++;
// }
// 	close(my_pipes.pipes[0]);
// 	close(my_pipes.pipes[1]);
// 	close(my_pipes.pipes[2]);
// 	close(my_pipes.pipes[3]);
// 	execute_command(pid, node, envp);
// }
// }


// void execute_pipe(t_node *node, char *envp[])
// {
// 	int	pipe_amount;
// 	t_pipes my_pipes;
// 	static int step = 0;
// 	int i = 0;
// 	static int j = 0;
// 	static int k = 1;
// 	static int counter;
// 	int	pid;
// if (step == 0)
// {
// 	pipe_amount = get_pipe_amount(node);
// 	pipe_amount = pipe_amount * 2;
// 	step = 1;
// 	my_pipes.pipes = malloc(sizeof(int) * pipe_amount);
// 	while (i < pipe_amount)
// 	{
// 		pipe(&my_pipes.pipes[i]);
// 		i = i + 2;
// 	}
// 	counter = pipe_amount / 2;
// 	counter = counter + 1;
// }
// // if (step <= counter)
// // {

// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		if (step == 1)
// 		{
// 			printf("wehere\n");
// 			dup2(my_pipes.pipes[1], STDOUT_FILENO);
// 			close(my_pipes.pipes[0]);
// 			close(my_pipes.pipes[1]);
// 	execute_command(pid, node, envp);
// 		}
// 		else if (step == 2)
// 		{
// 			printf("weherea\n");
// 		k = k + 2;
// 		dup2(my_pipes.pipes[0], STDIN_FILENO);
// 		dup2(my_pipes.pipes[3], STDOUT_FILENO);
// 			close(my_pipes.pipes[0]);
// 			close(my_pipes.pipes[1]);
// 			close(my_pipes.pipes[2]);
// 			close(my_pipes.pipes[3]);
// 	execute_command(pid, node, envp);
// 		}
// 		else if (step == 3)
// 		{
// 			printf("weherei\n");
// 			j = j + 2;
// 			dup2(my_pipes.pipes[2], STDIN_FILENO);
// 			close(my_pipes.pipes[2]);
// 			close(my_pipes.pipes[3]);
// 	execute_command(pid, node, envp);
// 		}

// 	}
// 	else
// 	{
// 		if (step == 3)
// 		{
// 			close(my_pipes.pipes[0]);
// 			close(my_pipes.pipes[1]);
// 			close(my_pipes.pipes[2]);
// 			close(my_pipes.pipes[3]);
// 		}
// 		waitpid(pid, NULL, 0);
// 		step++;
// 	}
// //}
// }

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
		dup2(my_pipes.pipes[k], STDOUT_FILENO);
}
if (step != 1)
{
		dup2(my_pipes.pipes[j], STDIN_FILENO);
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

// void execute_pipe(t_node *node, char *envp[])
// {
// 	int	pipe_amount;
// 	pipe_amount = get_pipe_amount(node);
// 	pipe_amount = pipe_amount * 2;
// 	t_pipes my_pipes;
// 	static int step = 0;
// 	int i = 0;
// 	int	pid;
// if (step == 0)
// {
// 	step = 1;
// 	my_pipes.pipes = malloc(sizeof(int) * pipe_amount);
// 	while (i < pipe_amount)
// 	{
// 		pipe(&my_pipes.pipes[i]);
// 		i = i + 2;
// 	}
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		dup2(my_pipes.pipes[1], STDOUT_FILENO);
// 		close(my_pipes.pipes[0]);
// 		close(my_pipes.pipes[1]);
// 		close(my_pipes.pipes[2]);
// 		close(my_pipes.pipes[3]);
// 		execute_command(pid, node, envp);
// 	}
// }
// else if (step == 1)
// {
// 	step = 2;
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		dup2(my_pipes.pipes[0], STDIN_FILENO);
// 		dup2(my_pipes.pipes[3], STDOUT_FILENO);
// 		close(my_pipes.pipes[0]);
// 		close(my_pipes.pipes[1]);
// 		close(my_pipes.pipes[2]);
// 		close(my_pipes.pipes[3]);
// 		execute_command(pid, node, envp);
// 	}
// }
// else if (step == 2)
// {
// 	step = 3;
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 	dup2(my_pipes.pipes[2], STDIN_FILENO);
// 	close(my_pipes.pipes[0]);
// 	close(my_pipes.pipes[1]);
// 	close(my_pipes.pipes[2]);
// 	close(my_pipes.pipes[3]);
// 	execute_command(pid, node, envp);
// 	}
// 	close(my_pipes.pipes[0]);
// 	close(my_pipes.pipes[1]);
// 	close(my_pipes.pipes[2]);
// 	close(my_pipes.pipes[3]);
// }

// }

void	loop_nodes(t_node *list, char *envp[])
{
	t_node *curr;

	curr = list;
	while (curr != NULL)
	{
//if next is NULL, then simply execute the command
//this might need another check here?
		if (curr->next == NULL && curr->prev->type != PIPE)
		{
			execute_command(-1, curr, envp);
		}
//do command and direct it to outfile
		else if (curr->next != NULL && curr->next->type == REDIR_APPEND)
		{
			redirection_outfile(curr, envp);
			curr = curr->next;
		}
//do command and append it to outfile
		else if (curr->next != NULL && curr->next->type == REDIR_OUTF)
		{
			redirection_outfile(curr, envp);
			curr = curr->next;
		}
//WIPWIP!!
		else if (curr->next != NULL && curr->next->type == REDIR_INF)
		{
			redirection_infile(curr, envp);
			curr = curr->next;
		}
//do pipe
		else if (curr->next != NULL && curr->next->type == PIPE)
		{
			execute_pipe(curr, envp);
			curr = curr->next;
		}
		else if (curr->next == NULL && curr->prev->type == PIPE)
		{
			execute_pipe(curr, envp);
		}
		curr = curr->next;
	}
}

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
	node4->cmd = "grep";
	node4->args = malloc(sizeof(char *) * 2);
	node4->args[0] = "test";
	node4->args[1] = NULL;

	t_node *node5;
	node5 = malloc(sizeof(t_node));
	node5->type = PIPE;
	node5->file = NULL;
	node5->cmd = NULL;
	node5->args = NULL;

	t_node *node6;
	node6 = malloc(sizeof(t_node));
	node6->type = COMMAND;
	node6->file = NULL;
	node6->cmd = "wc";
	node6->args = malloc(sizeof(char *) * 2);
	node6->args[0] = "-l";
	node6->args[1] = NULL;


	node0->next = node1;
	node0->prev  = NULL;
	node1->next = node2;
	node1->prev = node0;
	node2->next = node3;
	node2->prev = node1;
	node3->next = node4;
	node3->prev = node2;
	node4->next = node5;
	node4->prev = node3;
	node5->next = node6;
	node5->prev = node4;
	node6->next = NULL;
	node6->prev = node5;

	loop_nodes(node0, envp);
	return (0);

}


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
// 	node0->cmd = "echo";
// 	node0->args = malloc(sizeof(char *) * 2);
// 	node0->args[0] = "hi there!";
// 	node0->args[1] = NULL;

// 	t_node *node1;
// 	node1 = malloc(sizeof(t_node));
// 	node1->type = REDIR_OUTF;
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

// }

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



















// # include "../libft/includes/libft.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <readline/readline.h>
// #include <readline/history.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <fcntl.h>

// typedef enum s_type
// {
//     PIPE,
//     COMMAND,
//     REDIR_INF,
//     REDIR_OUTF,
//     REDIR_APPEND,
//     REDIR_HEREDOC,
// }            t_type;

// typedef struct s_node
// {
//     t_type            type;
//     char            *file;
//     char            **args;
//     char            *cmd;
//     struct s_node    *prev;
//     struct s_node    *next;
// }                    t_node;

// int	set_outfile(char *file, int append);
// int	open_infile(char *file);
// char **get_paths(char *envp[]);
// char *get_absolute_path(char **paths, char *command);

// char **combine_commands(t_node *node)
// {
// int i = 0;
// char **commands;
// while (node->args[i] != NULL)
// {
// 	i++;
// }

// commands = malloc(sizeof(char *) * (i + 2));
// commands[i + 1] = NULL;
// commands[0] = strdup(node->cmd);
// int j = 0;
// while (j < i)
// {
// 	commands[j + 1] = strdup(node->args[j]);
// 	j++;
// }
// return (commands);
// }


// void execute_command(int pid, char *path, t_node *node, char *envp[])
// {
// //doube check the pid value
// 	char **commands;
// 	commands = combine_commands(node);
// 	if (pid == -1)
// 		pid = fork();
// 	if (pid == 0)
// 	{
// 		execve(path, commands, envp);
// 	}
// 	else
// 	{
// 		waitpid(pid, NULL, 0);
// 	}
// }

// void redirection_outfile(char *path, t_node *node, char *envp[])
// {
// 	int pid;
// 	pid = fork();
// 	if (pid == 0)
// {
// 	int fd;

// 	if (node->next->type == REDIR_OUTF)
// 		fd = set_outfile(node->next->file, 0);
// 	if (node->next->type == REDIR_APPEND)
// 		fd = set_outfile(node->next->file, 1);
// 	dup2(fd, STDOUT_FILENO);
// 	close(fd);
// 	execute_command(pid, path, node, envp);
// }
// }

// void	loop_nodes(t_node *list, char *absolute_path, char *envp[])
// {
// 	t_node *curr;

// 	curr = list;
// 	while (curr != NULL)
// 	{
// //if next is NULL, then simply execute the command
// 		if (curr->next == NULL)
// 		{
// 			execute_command(-1, absolute_path, curr, envp);
// 		}
// //do command and direct it to outfile
// 		else if (curr->next->type == REDIR_APPEND)
// 		{
// 			redirection_outfile(absolute_path, curr, envp);
// 			curr = curr->next;
// 		}
// //do command and append it to outfile
// 		else if (curr->next->type == REDIR_OUTF)
// 		{
// 			redirection_outfile(absolute_path, curr, envp);
// 			curr = curr->next;
// 		}
// 		curr = curr->next;
// 	}
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
