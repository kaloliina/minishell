#include "minishell.h"

int	add_existing_envp(char ***new_envp, char **envp, t_pipes *my_pipes)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		(*new_envp)[i] = ft_strdup(envp[i]);
		if (!(*new_envp)[i])
		{
			free_array((*new_envp));
			handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
		}
		i++;
	}
	return (i);
}

int	is_valid_to_export(char *arg)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(arg[i]) || arg[i] == '_'))
		return (0);
	while (!is_exp_delimiter(arg[i]))
		i++;
	if (arg[i] != '=' && arg[i] != '\0')
		return (0);
	return (1);
}

//add exported element(s) to envp
int	add_exported_envp(char ***new_envp, char **cmd, int i, t_pipes *my_pipes)
{
	int	j;

	j = 1;
	while (cmd[j])
	{
		if (is_valid_to_export(cmd[j]))
		{
			(*new_envp)[i] = ft_strdup(cmd[j]);
			if (!(*new_envp)[i])
			{
				free_array((*new_envp));
				handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
			}
			i++;
		}
		j++;
	}
	return (i);
}

//go through envp to find the element to unset
int	find_unset_element(char *arg, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && arg)
	{
		if (!ft_strncmp(envp[i], arg, ft_strlen(arg))
			&& envp[i][ft_strlen(arg)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	**fill_unset_envp(char ***new_envp, char **cmd,
	char **envp, t_pipes *my_pipes)
{
	int	i;
	int	j;
	int	k;
	int	element;

	i = 0;
	j = 1;
	k = 0;
	while (cmd[j])
	{
		element = find_unset_element(cmd[j], envp);
		if (element == -1)
			j++;
		else
			break ;
	}
	while ((envp)[i])
	{
		if (i == element)
		{
			i++;
			j++;
			element = find_unset_element(cmd[j], envp);
		}
		else
		{
			(*new_envp)[k] = ft_strdup(envp[i]);
			if (!(*new_envp)[k])
			{
				free_array((*new_envp));
				handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
			}
			i++;
			k++;
		}
	}
	(*new_envp)[k] = NULL;
	return (*new_envp);
}
