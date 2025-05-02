#include "minishell.h"

static int	find_first_unset_element(char **cmd, char **envp, int j)
{
	int	element;

	while (cmd[j])
	{
		element = find_unset_element(cmd[j], envp);
		if (element == -1)
			j++;
		else
			break ;
	}
	return (element);
}

static int	find_next_unset_element(int *i, int *j, char **cmd, char **envp)
{
	(*i)++;
	(*j)++;
	return (find_unset_element(cmd[*j], envp));
}

static int	find_unset_element(char *arg, char **envp)
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

static char	**fill_unset_envp(char ***new_envp, char **cmd,
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

void	execute_unset(char **cmd, char ***envp, t_pipes *my_pipes)
{
	int		i;
	int		j;
	int		args;
	char	**new_envp;

	if (!cmd[1])
		return ;
	i = count_elements(*envp);
	j = 1;
	args = 0;
	while (cmd[j])
	{
		if (find_unset_element(cmd[j], *envp) != -1)
			args++;
		j++;
	}
	if (!args)
		return ;
	new_envp = malloc(sizeof(char *) * ((i - args) + 2));
	if (!new_envp)
		fatal_exec_error(MALLOC, my_pipes, NULL, NULL);
	new_envp = fill_unset_envp(&new_envp, cmd, *envp, my_pipes);
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
}
