/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:10:47 by sojala            #+#    #+#             */
/*   Updated: 2025/05/04 18:10:48 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	initialize_struct(t_pipes *my_pipes, t_node *list, char ***envp)
{
	int	i;

	*my_pipes = (t_pipes){NULL, NULL, get_pipe_amount(list),
		1, -1, -1, -1, -1, 0, 1, 0, 0, NULL, NULL, envp, NULL, NULL};
	if (my_pipes->pipe_amount > 0)
	{
		my_pipes->pipes = ft_calloc((my_pipes->pipe_amount * 2), sizeof(int));
		if (my_pipes->pipes == NULL)
			fatal_exec_error(ERR_MALLOC, my_pipes, list, NULL);
		i = 0;
		while (i < my_pipes->pipe_amount)
		{
			if (pipe(&my_pipes->pipes[(i++) * 2]) < 0)
				fatal_exec_error(ERR_PIPE, my_pipes, list, NULL);
		}
	}
	my_pipes->childpids = ft_calloc((my_pipes->pipe_amount + 1), sizeof(pid_t));
	if (my_pipes->childpids == NULL)
		fatal_exec_error(ERR_MALLOC, my_pipes, list, NULL);
	my_pipes->stdoutfd = dup(STDOUT_FILENO);
	if (my_pipes->stdoutfd == -1)
		fatal_exec_error(ERR_FD, my_pipes, list, NULL);
	my_pipes->stdinfd = dup(STDIN_FILENO);
	if (my_pipes->stdinfd == -1)
		fatal_exec_error(ERR_FD, my_pipes, list, NULL);
}

static int	finalize_execution(int amount, t_pipes *my_pipes)
{
	int	exit_status;
	int	i;

	exit_status = 0;
	i = 0;
	while (i < amount)
	{
		if (my_pipes->childpids[i] > 0)
		{
			if (waitpid(my_pipes->childpids[i], &exit_status, 0) < 0)
				fatal_exec_error(ERR_WAITPID, my_pipes, NULL, NULL);
			if (WIFEXITED(exit_status))
				exit_status = WEXITSTATUS(exit_status);
			else if (WIFSIGNALED(exit_status))
				exit_status = 128 + WTERMSIG(exit_status);
		}
		else
			exit_status = my_pipes->exit_status;
		i++;
	}
	if (dup2(my_pipes->stdinfd, STDIN_FILENO) < 0
		|| dup2(my_pipes->stdoutfd, STDOUT_FILENO) < 0)
		fatal_exec_error(ERR_FD, my_pipes, NULL, NULL);
	free_my_pipes(my_pipes);
	return (exit_status);
}

static void	loop_nodes(t_node *list, int status, t_pipes *my_pipes)
{
	if (list->type == COMMAND)
		my_pipes->command_node = list;
	if (list->file && (list->type == REDIR_APPEND || list->type == REDIR_OUTF))
		set_outfile(list->file, list->type, my_pipes);
	if (list->file && list->type == REDIR_INF)
		open_infile(list->file, my_pipes);
	if (list->type == REDIR_HEREDOC)
	{
		my_pipes->heredoc_node = list;
		if (heredoc(list, my_pipes, status) < 0)
			return ;
	}
	if ((list->next == NULL) || (list->next && my_pipes->pipe_amount > 0
			&& list->next->type == PIPE))
	{
		if (my_pipes->command_node != NULL
			&& is_builtin(my_pipes->command_node->cmd[0]) == 1)
			my_pipes->childpids[my_pipes->current_section -1]
				= execute_builtin(my_pipes->command_node, my_pipes);
		else if (my_pipes->command_node != NULL)
			my_pipes->childpids[my_pipes->current_section -1]
				= execute_executable(my_pipes->command_node, my_pipes);
		close_pipeline_fds(my_pipes);
	}
}

int	begin_execution(t_node *list, char ***envp, int status)
{
	t_pipes	*my_pipes;

	my_pipes = malloc(sizeof(t_pipes));
	if (my_pipes == NULL)
	{
		free_array(*envp);
		fatal_exec_error(ERR_MALLOC, my_pipes, list, NULL);
	}
	initialize_struct(my_pipes, list, envp);
	while (list != NULL)
	{
		loop_nodes(list, status, my_pipes);
		if (my_pipes->exit_status == 130)
		{
			free_my_pipes(my_pipes);
			return (130);
		}
		list = list->next;
	}
	return (finalize_execution(my_pipes->current_section, my_pipes));
}
