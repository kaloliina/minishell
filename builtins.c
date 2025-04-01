#include "minishell.h"

void	execute_echo(t_node *node, char **envp)
{
	int		i;
	char	*temp;

	i = 1;
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
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

void	execute_pwd()
{
	char	*buf;

	buf = malloc(100);
	if (!buf)
	{
		printf("minishell: memory allocation failure\n");
		exit (1);
	}
	getcwd(buf, 100);
	if (!buf)
		perror("minishell:");
	else
		printf("%s\n", buf);
	free (buf);
}

void	execute_export(char **cmd, char ***envp)
{
	int		i;
	int		j;
	int		args;
	char	**new_envp;

	i = 0;
	while ((*envp)[i])
		i++;
	args = 1;
	while (cmd[args + 1])
		args++;
	new_envp = malloc(sizeof(char *) * (i + 1 + args));
	//malloc protection
	i = 0;
	while ((*envp)[i])
	{
		new_envp[i] = ft_strdup((*envp)[i]);
		//malloc protection
		i++;
	}
	j = 1;
	while (cmd[j])
	{
		new_envp[i] = ft_strdup(cmd[j]);
		i++;
		j++;
		//malloc protection
	}
	new_envp[i] = NULL;
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
}
