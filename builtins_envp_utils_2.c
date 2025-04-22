#include "minishell.h"

int	export_validation(char **cmd, int i)
{
	int	j;

	while (cmd[i])
	{
		j = 0;
		if (!(ft_isalpha(cmd[i][j]) || cmd[i][j] == '_'))
		{
			ft_printf(2, EXPORT, cmd[i]);
			return (-1);
		}
		while (cmd[i][j])
		{
			while (cmd[i][j] && !is_exp_delimiter(cmd[i][j]))
				j++;
			if (cmd[i][j] != '=' && cmd[i][j] != '\0')
			{
				ft_printf(2, EXPORT, cmd[i]);
				return (-1);
			}
			j = ft_strlen(cmd[i]);
		}
		i++;
	}
	return (0);
}

int	count_args_to_export(char **cmd)
{
	int	j;
	int	args;

	j = 1;
	args = 0;
	while (cmd[j])
	{
		if (is_valid_to_export(cmd[j]))
			args++;
		j++;
	}
	return (args);
}

int	find_first_unset_element(char **cmd, char **envp, int j)
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

int	find_next_unset_element(int *i, int *j, char **cmd, char **envp)
{
	int	element;

	(*i)++;
	(*j)++;
	return (find_unset_element(cmd[*j], envp));
}

void	handle_fatal_envp_exit(char **new_envp, t_pipes *my_pipes)
{
	free_array(new_envp);
	handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
}
