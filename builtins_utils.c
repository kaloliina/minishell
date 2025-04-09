#include "minishell.h"

//fill new envp with existing elements
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

//add exported element(s) to envp
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

char	**sort_for_export(char **export, char **envp, int elements)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (i < elements)
	{
		j = 0;
		k = 0;
		while (j < elements)
		{
			if (ft_strcmp(envp[i], envp[j]) > 0)
				k++;
			j++;
		}
		export[k] = ft_strdup(envp[i]);
		if (!export[k])
		{
			ft_printf(2, MALLOC);
			free_array(export);
			exit (1);
		}
		i++;
	}
	return (export);
}

char	**fill_unset_envp(char **new_envp, char **cmd, char **envp)
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
				ft_printf(2, MALLOC);
				free_array(new_envp);
				exit (1);
			}
			i++;
			k++;
		}
	}
	new_envp[k] = NULL;
	return (new_envp);
}
