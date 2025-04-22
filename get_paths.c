#include "../minishell.h"

//this one check again XD you did weird changes
char	**get_paths(t_pipes *my_pipes)
{
	int		i = 0;
	char	**paths;
	char	*string;

	string = NULL;
	while ((*my_pipes->my_envp)[i] != NULL)
	{
		if (ft_strncmp((*my_pipes->my_envp)[i], "PATH=", 5) == 0)
		{
			string = ft_substr((*my_pipes->my_envp)[i], 5, (ft_strlen((*my_pipes->my_envp)[i]) - 5));
			if (string == NULL)
				handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
			break;
		}
		i++;
	}
	if (string == NULL)
		return (NULL);
	paths = ft_split(string, ':');
	if (paths == NULL)
	{
		free (string);
		handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
	}
	free (string);
	return (paths);
}

//we need to include my pipes in this because now we return NULL in case of malloc fail and if we didnt find it
char	*get_absolute_path(char **paths, char *command)
{
	char	*path_helper = NULL;
	char	*path = NULL;
	int		i = 0;

	if (ft_strchr(command, '/'))
	{
		path = ft_strdup(command);
		//malloc check
		return (path);
	}
	while (paths != NULL && paths[i] != NULL)
	{
		path_helper = ft_strjoin(paths[i], "/");
		if (path_helper == NULL)
			return (NULL);
		path = ft_strjoin(path_helper, command);
		if (path == NULL)
		{
			free (path_helper);
			return (NULL);
		}
		if (access(path, F_OK | X_OK) == 0)
		{
			free (path_helper);
			return (path);
		}
		free (path_helper);
		free (path);
		i++;
	}
	return (NULL);
}
