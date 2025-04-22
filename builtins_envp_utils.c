#include "minishell.h"

int	add_existing_envp(char ***new_envp, char **envp, t_pipes *my_pipes)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		(*new_envp)[i] = ft_strdup(envp[i]);
		if (!(*new_envp)[i])
			handle_fatal_envp_exit(*new_envp, my_pipes);
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
				handle_fatal_envp_exit(*new_envp, my_pipes);
			i++;
		}
		j++;
	}
	return (i);
}

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
	element = find_first_unset_element(cmd, envp, j);
	while ((envp)[i])
	{
		if (i == element)
			element = find_next_unset_element(&i, &j, cmd, envp);
		else
		{
			(*new_envp)[k] = ft_strdup(envp[i]);
			if (!(*new_envp)[k])
				handle_fatal_envp_exit(*new_envp, my_pipes);
			i++;
			k++;
		}
	}
	(*new_envp)[k] = NULL;
	return (*new_envp);
}
