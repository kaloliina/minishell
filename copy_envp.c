#include "minishell.h"

char	**copy_envp(char **envp)
{
	char	**my_envp;
	int		i;

	i = count_elements(envp);
	my_envp = malloc(sizeof(char *) * (i + 1));
	if (!my_envp)
	{
		ft_printf(2, "%s", MALLOC);
		exit (1);
	}
	i = 0;
	while (envp[i])
	{
		my_envp[i] = ft_strdup(envp[i]);
		if (!my_envp[i])
		{
			ft_printf(2, "%s", MALLOC);
			free_array(my_envp);
			exit (1);
		}
		i++;
	}
	my_envp[i] = NULL;
	return (my_envp);
}
