#include "minishell.h"

void	execute_echo(t_node *node, char **envp)
{
	int		i;
	char	*temp;

	i = 1;
	printf("here?\n");
	while (node->cmd[i])
	{
		temp = handle_expandables(node->cmd[i], envp);
		if (temp)
			node->cmd[i] = temp;
		printf("%s", node->cmd[i]);
		if (node->cmd[i + 1])
			printf(" ");
		i++;
	}
	printf("\n");
}

void	execute_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		printf("%s\n", envp[i++]);
}
