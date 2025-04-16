#include "minishell.h"

static char	**sort_for_export(char **export, char **envp, int elements, t_pipes *my_pipes)
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
			free_array(export);
			handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
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

	elements = count_elements(envp);
	export = malloc(sizeof(char *) * (elements + 1));
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
	i = count_elements(*envp);
	args = 1;
	while (cmd[args + 1])
		args++;
	new_envp = malloc(sizeof(char *) * (i + 1 + args));
	if (!new_envp)
		handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
	i = add_existing_envp(new_envp, *envp, my_pipes);
	i = add_exported_envp(new_envp, cmd, i, my_pipes);
	new_envp[i] = NULL;
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
}

void	execute_unset(char **cmd, char ***envp, t_pipes *my_pipes)
{
	int		i;
	int		args;
	char	**new_envp;

	i = count_elements(*envp);
	args = 1;
	while (cmd[args + 1])
		args++;
	new_envp = malloc(sizeof(char *) * ((i - args) + 2));
	if (!new_envp)
		handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
	new_envp = fill_unset_envp(new_envp, cmd, *envp, my_pipes);
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
}
