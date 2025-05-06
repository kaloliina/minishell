/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_pwd_cd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:11:43 by sojala            #+#    #+#             */
/*   Updated: 2025/05/06 10:51:41 by sojala           ###   ########.fr       */
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
			free (envp[j++]);
		if (j == i)
			j++;
		while (envp[j])
			free (envp[j++]);
		free (envp);
		envp = NULL;
	}
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
		free_expand(expand);
	exit (exit_status);
}
