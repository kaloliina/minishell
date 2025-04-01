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
		ft_printf(1, "%s", node->cmd[i]);
		if (node->cmd[i + 1])
			ft_printf(1, " ");
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
		ft_printf(1, "%s\n", envp[i]);
		i++;
	}
}

void	execute_pwd()
{
	char	*buf;

	buf = malloc(100);
	if (!buf)
	{
		ft_printf(2, "minishell: memory allocation failure\n");
		exit (1);
	}
	getcwd(buf, 100);
	if (!buf)
		perror("minishell");
	else
		ft_printf(1, "%s\n", buf);
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

void	execute_cd(char **cmd)
{
	if (cmd[2])
	{
		ft_printf(2, "minishell: cd: too many arguments\n");
		//free everything
		exit (1);
	}
	if (chdir(cmd[1]) == -1)
		perror("minishell");
}

void	execute_unset(char **cmd, char ***envp)
{
	int		i;
	int		j;
	int		k;
	int		args;
	char	**new_envp;

	i = 0;
	while ((*envp)[i])
		i++;
	args = 1;
	while (cmd[args + 1])
		args++;
	new_envp = malloc(sizeof(char *) * ((i - args) + 2));
	//malloc protection
	i = 0;
	k = 0;
	while ((*envp)[i])
	{
		j = 1;
		while (cmd[j])
		{
			if (!ft_strncmp((*envp)[i], cmd[j], ft_strlen(cmd[j])))
				break ;
			j++;
		}
		if (cmd[j] && !ft_strncmp((*envp)[i], cmd[j], ft_strlen(cmd[j])))
			i++;
		else
		{
			new_envp[k] = ft_strdup((*envp)[i]);
			//malloc protection
			i++;
			k++;
		}
	}
	new_envp[k] = NULL;
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
}
