/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:10:00 by sojala            #+#    #+#             */
/*   Updated: 2025/05/04 18:10:01 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_envp_array(char **envp, int i)
{
	int	j;

	j = 0;
	if (envp)
	{
		while (envp[j])
		{
			if (envp[j])
				free (envp[j++]);
		}
		if (j == i)
			j++;
		while (envp[j])
		{
			if (envp[j])
				free (envp[j++]);
		}
	}
	free (envp);
}

void	fatal_pwd_error(char *msg, t_pipes *my_pipes, int i, t_exp *expand)
{
	int	exit_status;

	exit_status = 1;
	if (msg)
		print_error(msg, NULL, NULL);
	if (my_pipes->command_node)
		free_nodes(my_pipes->command_node);
	else
		free_nodes(my_pipes->heredoc_node);
	if (my_pipes)
	{
		exit_status = my_pipes->exit_status;
		if (my_pipes->hd_dir)
			handle_tmpfile(my_pipes);
		free_envp_array(*my_pipes->my_envp, i);
		free_my_pipes(my_pipes);
	}
	if (expand)
	{
		if (expand->exp)
			free (expand->exp);
		if (expand->expansion)
			free (expand->expansion);
	}
	exit (exit_status);
}

void	execute_pwd(t_pipes *my_pipes, char ***envp, int i, t_exp *expand)
{
	char	*buf;

	buf = malloc(4096);
	if (!buf)
		fatal_exec_error(ERR_MALLOC, my_pipes, NULL, NULL);
	if (!getcwd(buf, 4096))
		perror("minishell: pwd");
	else if (!envp && !i)
		ft_printf(1, "%s\n", buf);
	else
	{
		(*envp)[i] = ft_strjoin("PWD=", buf);
		if (!(*envp)[i])
		{
			free (buf);
			fatal_pwd_error(ERR_MALLOC, my_pipes, i, expand);
		}
	}
	free (buf);
}
