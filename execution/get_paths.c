/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_paths.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:08:15 by khiidenh          #+#    #+#             */
/*   Updated: 2025/03/17 08:58:35 by khiidenh         ###   ########.fr       */
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
	return (paths);
}

char *get_absolute_path(char **paths, char *command)
{
	char *path;
	int i = 0;
	command = ft_strjoin("/", command);
	while (paths[i] != NULL)
	{
		path = ft_strjoin(paths[i], command);
		if (access(path, F_OK | X_OK) == 0)
			break;
		i++;
	}
	return (path);
}
