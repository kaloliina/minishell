/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:10:58 by sojala            #+#    #+#             */
/*   Updated: 2025/05/04 18:10:59 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_pipe_amount(t_node *list)
{
	t_node	*curr;
	int		pipe_amount;

	pipe_amount = 0;
	curr = list;
	while (curr)
	{
		if (curr->type == PIPE)
			pipe_amount++;
		curr = curr->next;
	}
	return (pipe_amount);
}

int	is_builtin(char *command)
{
	const char	*builtins[7];
	int			i;

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	i = 0;
	while (i <= 6)
	{
		if (!ft_strcmp(builtins[i], command))
			return (1);
		i++;
	}
	return (0);
}

char	**get_paths(t_pipes *my_pipes)
{
	int		i;
	char	**paths;
	char	*string;

	i = 0;
	while ((*my_pipes->my_envp)[i] != NULL)
	{
		if (ft_strncmp((*my_pipes->my_envp)[i], "PATH=", 5) == 0)
		{
			string = ft_substr((*my_pipes->my_envp)[i], 5,
					(ft_strlen((*my_pipes->my_envp)[i]) - 5));
			if (string == NULL)
				fatal_exec_error(ERR_MALLOC, my_pipes, NULL, NULL);
			paths = ft_split(string, ':');
			free (string);
			if (paths == NULL)
				fatal_exec_error(ERR_MALLOC, my_pipes, NULL, NULL);
			return (paths);
		}
		i++;
	}
	return (NULL);
}

char	*get_absolute_path(char **paths, char *command, t_pipes *my_pipes)
{
	char	*path_helper;
	char	*path;

	if (ft_strchr(command, '/') || paths == NULL)
	{
		path = ft_strdup(command);
		if (path == NULL)
			fatal_exec_error(ERR_MALLOC, my_pipes, NULL, NULL);
		return (path);
	}
	while (paths != NULL && *paths != NULL)
	{
		path_helper = ft_strjoin(*paths, "/");
		if (path_helper == NULL)
			fatal_exec_error(ERR_MALLOC, my_pipes, NULL, NULL);
		path = ft_strjoin(path_helper, command);
		free (path_helper);
		if (path == NULL)
			fatal_exec_error(ERR_MALLOC, my_pipes, NULL, NULL);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		free (path);
		paths++;
	}
	return (NULL);
}
