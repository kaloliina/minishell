#include "minishell.h"

int	export_fill_envp(char ***new_envp, char **cmd, char **envp,
	t_pipes *my_pipes)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp[i])
	{
		j = find_existing_envp(new_envp, cmd, envp, i);
		if (j < 0)
			(*new_envp)[i] = ft_strdup(envp[i]);
		if (!(*new_envp)[i])
			fatal_export_unset_error(*new_envp, my_pipes);
		i++;
	}
	return (add_exported_envp(new_envp, cmd, i, my_pipes));
}

int	is_valid_to_export(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '_' && arg[i + 1] == '=')
		return (0);
	if (!(ft_isalpha(arg[i]) || arg[i] == '_'))
		return (0);
	while (!is_exp_delimiter(arg[i]))
		i++;
	if (arg[i] != '=')
		return (0);
	return (1);
}

int	add_exported_envp(char ***new_envp, char **cmd, int i,
	t_pipes *my_pipes)
{
	int	j;

	j = 1;
	while (cmd[j])
	{
		if (is_valid_to_export(cmd[j])
			&& !is_replacer_envp(my_pipes->my_envp, cmd[j]))
		{
			(*new_envp)[i] = ft_strdup(cmd[j]);
			if (!(*new_envp)[i])
				fatal_export_unset_error(*new_envp, my_pipes);
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
			&& envp[i][ft_strlen(arg)] == '='
			&& ft_strcmp(arg, "_"))
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
				fatal_export_unset_error(*new_envp, my_pipes);
			i++;
			k++;
		}
	}
	(*new_envp)[k] = NULL;
	return (*new_envp);
}
