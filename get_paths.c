/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_paths.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:08:15 by khiidenh          #+#    #+#             */
/*   Updated: 2025/04/01 14:37:56 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char **get_paths(char *envp[])
{
	int i = 0;
	char **paths;
	char *string;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			string = ft_substr(envp[i], 5, (ft_strlen(envp[i]) - 5));
			break;
		}
		i++;
	}
	paths = ft_split(string, ':');
	free (string);
	return (paths);
}

char *get_absolute_path(char **paths, char *command)
{
	char *path_helper = NULL;
	char *path = NULL;
	int i = 0;
	if (access(command, F_OK | X_OK) == 0)
		return (command);
	while (paths[i] != NULL)
	{
		path_helper = ft_strjoin(paths[i], "/");
		path = ft_strjoin(path_helper, command);
		if (access(path, F_OK | X_OK) == 0)
			break;
		free (path_helper);
		free (path);
		i++;
	}
	if (!paths[i])
	{
		ft_printf(2, "%s: command not found\n", command);
		exit (1);
	}
	if (path_helper)
		free (path_helper);
	return (path);
}
