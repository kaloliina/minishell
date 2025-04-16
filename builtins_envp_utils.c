#include "minishell.h"

int	add_existing_envp(char **new_envp, char **envp, t_pipes *my_pipes)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			free_array(new_envp);
			handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
		}
		i++;
	}
	return (i);
}

//add exported element(s) to envp
int	add_exported_envp(char **new_envp, char **cmd, int i, t_pipes *my_pipes)
{
	int	j;

	j = 1;
	while (cmd[j])
	{
		new_envp[i] = ft_strdup(cmd[j]);
		if (!new_envp[i])
		{
			free_array(new_envp);
			handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
		}
		i++;
		j++;
	}
	return (i);
}

//go through envp to find the element to unset
int	find_unset_element(char **cmd, char *envp_element)
{
	int	j;

	j = 1;
	while (cmd[j])
	{
		if (!ft_strncmp(envp_element, cmd[j], ft_strlen(cmd[j]))
			&& envp_element[ft_strlen(cmd[j])] == '=')
			break ;
		j++;
	}
	return (j);
}

char	**fill_unset_envp(char **new_envp, char **cmd,
	char **envp, t_pipes *my_pipes)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while ((envp)[i])
	{
		j = find_unset_element(cmd, envp[i]);
		if (cmd[j] && !ft_strncmp(envp[i], cmd[j], ft_strlen(cmd[j]))
			&& envp[i][ft_strlen(cmd[j])] == '=')
			i++;
		else
		{
			new_envp[k] = ft_strdup(envp[i]);
			if (!new_envp[k])
			{
				free_array(new_envp);
				handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
			}
			i++;
			k++;
		}
	}
	new_envp[k] = NULL;
	return (new_envp);
}
