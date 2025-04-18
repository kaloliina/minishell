#include "../minishell.h"

char	**get_paths(char ***envp)
{
	int		i = 0;
	char	**paths;
	char	*string;
	while ((*envp)[i] != NULL)
	{
		if (ft_strncmp((*envp)[i], "PATH=", 5) == 0)
		{
			string = ft_substr((*envp)[i], 5, (ft_strlen((*envp)[i]) - 5));
			if (string == NULL)
				return (NULL);
			break;
		}
		i++;
	}
	paths = ft_split(string, ':');
	if (paths == NULL)
	{
		free (string);
		return (NULL);
	}
	free (string);
	return (paths);
}

char	*get_absolute_path(char **paths, char *command)
{
	char	*path_helper = NULL;
	char	*path = NULL;
	int		i = 0;

	if (access(command, F_OK | X_OK) == 0)
	{
	path = ft_strdup(command);
	return (path);
	}
	while (paths[i] != NULL)
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
	path = ft_strdup(command);
	return (path);
}
