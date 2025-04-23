#include "../minishell.h"

char	**get_paths(t_pipes *my_pipes)
{
	int		i = 0;
	char	**paths;
	char	*string;

	while ((*my_pipes->my_envp)[i] != NULL)
	{
		if (ft_strncmp((*my_pipes->my_envp)[i], "PATH=", 5) == 0)
		{
			string = ft_substr((*my_pipes->my_envp)[i], 5, (ft_strlen((*my_pipes->my_envp)[i]) - 5));
			if (string == NULL)
				handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
			paths = ft_split(string, ':');
			free (string);
			if (paths == NULL)
				handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
			return (paths);
		}
		i++;
	}
	return (NULL);
}

//we need to include my pipes in this because now we return NULL in case of malloc fail and if we didnt find it
//also we need to add the check if paths is NULL, then strdup command
char	*get_absolute_path(char **paths, char *command)
{
	char	*path_helper;
	char	*path;
	int		i;

	i = 0;
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
		free (path_helper);
		if (path == NULL)
			return (NULL);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		free (path);
		i++;
	}
	return (NULL);
}
