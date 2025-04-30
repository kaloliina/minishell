#include "../minishell.h"

static void	run_builtin_command(t_node *node, t_pipes *my_pipes)
{
	if (!ft_strcmp(node->cmd[0], "echo"))
		execute_echo(node);
	else if (!ft_strcmp(node->cmd[0], "pwd"))
		execute_pwd(my_pipes);
	else if (!ft_strcmp(node->cmd[0], "env"))
		execute_env(my_pipes->my_envp);
	else if (!ft_strcmp(node->cmd[0], "export"))
		execute_export(node->cmd, my_pipes->my_envp, my_pipes);
	else if (!ft_strcmp(node->cmd[0], "unset"))
		execute_unset(node->cmd, my_pipes->my_envp, my_pipes);
	else if (!ft_strcmp(node->cmd[0], "exit"))
		execute_exit(node->cmd, my_pipes);
	else if (!ft_strcmp(node->cmd[0], "cd"))
		execute_cd(node->cmd, my_pipes);
}

static void	execute_builtin_child(t_node *node, t_pipes *my_pipes)
{
	listen_to_signals(0);
	handle_redirections(my_pipes);
	if (my_pipes->exit_status == 1)
	{
		free_nodes(my_pipes->command_node);
		free_array(*my_pipes->my_envp);
		free_my_pipes(my_pipes);
		exit (1);
	}
	run_builtin_command(node, my_pipes);
	free_nodes(my_pipes->command_node);
	free_array(*my_pipes->my_envp);
	free_my_pipes(my_pipes);
	exit (0);
}

int	execute_builtin(t_node *node, t_pipes *my_pipes)
{
	int	pid;

	if (my_pipes->pipe_amount > 0)
	{
		pid = fork();
		if (pid < 0)
			handle_fatal_exit(ERR_FORK, my_pipes, NULL, NULL);
		if (pid == 0)
			execute_builtin_child(node, my_pipes);
		else
		{
			listen_to_signals(1);
			return (pid);
		}
	}
	else
	{
		handle_redirections(my_pipes);
		if (my_pipes->exit_status == 1)
			return (0);
		run_builtin_command(node, my_pipes);
	}
	return (0);
}
