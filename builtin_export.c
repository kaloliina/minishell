#include "minishell.h"

static int	export_fill_envp(char ***new_envp, char **cmd, char **envp,
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

static int	add_exported_envp(char ***new_envp, char **cmd, int i,
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

static char	**sort_for_export(char **export, char **envp,
	int elements, t_pipes *my_pipes)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (i < elements)
	{
		j = 0;
		k = 0;
		if (ft_strncmp(envp[i], "_=", 2))
		{
			while (j < elements)
			{
				if (ft_strcmp(envp[i], envp[j]) > 0)
					k++;
				j++;
			}
			export[k] = ft_strdup(envp[i]);
			if (!export[k])
				fatal_sort_for_export_error(export, elements, my_pipes);
		}
		i++;
	}
	return (export);
}

static void	export_no_args(char **envp, t_pipes *my_pipes)
{
	int		elements;
	int		i;
	char	**export;

	elements = count_elements(envp) - 1;
	export = ft_calloc(sizeof(char *), (elements + 1));
	if (!export)
		fatal_exec_error(MALLOC, my_pipes, NULL, NULL);
	export = sort_for_export(export, envp, elements, my_pipes);
	export[elements] = NULL;
	i = 0;
	while (export[i])
		ft_printf(1, "declare -x %s\n", export[i++]);
	free_array(export);
}

void	execute_export(char **cmd, char ***envp, t_pipes *my_pipes)
{
	int		i;
	int		args;
	char	**new_envp;

	if (!cmd[1])
		return (export_no_args(*envp, my_pipes));
	if (export_validation(cmd, 1) < 0)
		my_pipes->exit_status = 1;
	i = count_elements(*envp);
	args = count_args_to_export(cmd);
	if (!args)
		return ;
	new_envp = malloc(sizeof(char *) * (i + 1 + args));
	if (!new_envp)
		fatal_exec_error(MALLOC, my_pipes, NULL, NULL);
	i = export_fill_envp(&new_envp, cmd, *envp, my_pipes);
	new_envp[i] = NULL;
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
}
