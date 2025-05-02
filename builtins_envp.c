#include "minishell.h"

void	fatal_export_failure(char **export, int elements, t_pipes *my_pipes)
{
	int	i;

	i = 0;
	while (i < elements)
	{
		if (export[i])
			free (export[i]);
		i++;
	}
	free (export);
	export = NULL;
	handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
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
				fatal_export_failure(export, elements, my_pipes);
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
		handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
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
		handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
	i = export_fill_envp(&new_envp, cmd, *envp, my_pipes);
	new_envp[i] = NULL;
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
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
		handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
	new_envp = fill_unset_envp(&new_envp, cmd, *envp, my_pipes);
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
}
