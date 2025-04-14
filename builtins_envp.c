#include "minishell.h"

static char	**sort_for_export(char **export, char **envp, int elements)
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

void	export_no_args(char **envp)
{
	int	elements;
	int	i;
	char	**export;

	elements = count_elements(envp);
	export = malloc(sizeof(char *) * (elements + 1));
	//malloc protection
	export = sort_for_export(export, envp, elements);
	export[elements] = NULL;
	i = 0;
	while (export[i])
		ft_printf(1, "declare -x %s\n", export[i++]);
	free_array(export);
}

void	execute_export(char **cmd, char ***envp)
{
	int		i;
	int		args;
	char	**new_envp;

	if (!cmd[1])
		return (export_no_args(*envp));
	i = count_elements(*envp);
	args = 1;
	while (cmd[args + 1])
		args++;
	new_envp = malloc(sizeof(char *) * (i + 1 + args));
	//malloc protection
	i = add_existing_envp(new_envp, *envp);
	i = add_exported_envp(new_envp, cmd, i);
	new_envp[i] = NULL;
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
}

void	execute_unset(char **cmd, char ***envp)
{
	int		i;
	int		args;
	char	**new_envp;

	i = count_elements(*envp);
	args = 1;
	while (cmd[args + 1])
		args++;
	new_envp = malloc(sizeof(char *) * ((i - args) + 2));
	//malloc protection
	new_envp = fill_unset_envp(new_envp, cmd, *envp);
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
}
