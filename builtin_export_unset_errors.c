#include "minishell.h"

void	fatal_export_unset_error(char **new_envp, t_pipes *my_pipes)
{
	free_array(new_envp);
	fatal_exec_error(ERR_MALLOC, my_pipes, NULL, NULL);
}

void	fatal_sort_for_export_error(char **export, int elements,
	t_pipes *my_pipes)
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
	fatal_exec_error(ERR_MALLOC, my_pipes, NULL, NULL);
}
