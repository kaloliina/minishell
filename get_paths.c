#include "../minishell.h"

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

char	*get_absolute_path(char **paths, char *command, t_pipes *my_pipes)
{
	char	*path_helper;
	char	*path;

	if (ft_strchr(command, '/') || paths == NULL)
	{
		path = ft_strdup(command);
		if (path == NULL)
			handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
		return (path);
	}
	while (paths != NULL && *paths != NULL)
	{
		path_helper = ft_strjoin(*paths, "/");
		if (path_helper == NULL)
			handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
		path = ft_strjoin(path_helper, command);
		free (path_helper);
		if (path == NULL)
			handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		free (path);
		paths++;
	}
	return (NULL);
}
