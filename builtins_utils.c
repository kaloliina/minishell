#include "minishell.h"

int	fill_new_envp(char ***new_envp, char **envp, char **cmd, int args)
{
	int		i;
	int		j;

	i = 0;
	while ((envp)[i])
	{
		(*new_envp)[i] = ft_strdup(envp[i]);
		if (!(*new_envp)[i])
			return (-1);
		i++;
	}
	j = 1;
	while (cmd[j])
	{
		(*new_envp)[i] = ft_strdup(cmd[j]);
		if (!(*new_envp)[i])
			return (-1);
		i++;
		j++;
	}
	(*new_envp)[i] = NULL;
	return (0);
}

int	add_existing_envp(char **new_envp, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		//malloc protection
		i++;
	}
	return (i);
}

int	add_exported_envp(char **new_envp, char **cmd, int i)
{
	int	j;

	j = 1;
	while (cmd[j])
	{
		new_envp[i] = ft_strdup(cmd[j]);
		//malloc protection
		i++;
		j++;
	}
	return (i);
}

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
