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
		//malloc protection
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
			ft_printf(2, "%s\n", MALLOC);
			free_array(export); //must also free everything else
			exit (1);
		}
		i++;
	}
	return (export);
}
