#include "minishell.h"

void	execute_env(char ***envp)
{
	int	i;

	i = 0;
	while ((*envp)[i])
	{
		ft_printf(1, "%s\n", (*envp)[i]);
		i++;
	}
}
