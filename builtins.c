#include "minishell.h"

void	execute_echo(t_node *node, char **envp)
{
	int		i;
	char	*temp;

	i = 1;
	if (!ft_strcmp(node->cmd[1], "-n"))
		i++;
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
	if (ft_strcmp(node->cmd[1], "-n"))
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

void	execute_pwd(void)
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

static int	fill_new_envp(char ***new_envp, char **envp, char **cmd, int args)
{
	int		i;
	int		j;

	i = 0;
	while ((envp)[i])
	{
		(*new_envp)[i] = ft_strdup(envp[i]);
		if (!(*new_envp)[i])
			return (-1);
		i++;
	}
	j = 1;
	while (cmd[j])
	{
		(*new_envp)[i] = ft_strdup(cmd[j]);
		if (!(*new_envp)[i])
			return (-1);
		i++;
		j++;
	}
	(*new_envp)[i] = NULL;
	return (0);
}

int	execute_export(char **cmd, char ***envp)
{
	int		i;
	int		j;
	int		args;
	char	**new_envp;

	i = count_elements(*envp);
	args = 1;
	while (cmd[args + 1])
		args++;
	new_envp = malloc(sizeof(char *) * (i + 1 + args));
	if (!new_envp)
		return (-1);
	if (fill_new_envp(&new_envp, *envp, cmd, args) == -1)
	{
		free_array(new_envp);
		return (-1);
	}
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
	return (0);
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

int	fill_unset_envp(char ***new_envp, char **envp, char **cmd, int args)
{
	int	i;
	int	j;
	int	k;

	k = 0;
	i = 0;
	while (envp[i])
	{
		j = 1;
		while (cmd[j] && ft_strncmp(envp[i], cmd[j], ft_strlen(cmd[j])))
			j++;
		if (cmd[j] && !ft_strncmp(envp[i], cmd[j], ft_strlen(cmd[j])))
			i++;
		else
		{
			(*new_envp)[k] = ft_strdup(envp[i]);
			if (!(*new_envp)[k])
				return (-1);
			i++;
			k++;
		}
	}
	(*new_envp)[k] = NULL;
	return (0);
}

int	execute_unset(char **cmd, char ***envp)
{
	int		i;
	int		j;
	int		k;
	int		args;
	char	**new_envp;

	i = count_elements(*envp);
	args = 1;
	while (cmd[args + 1])
		args++;
	new_envp = malloc(sizeof(char *) * ((i - args) + 2));
	if (!new_envp)
		return (-1);
	i = 0;
	k = 0;
	if (fill_unset_envp(&new_envp, *envp, cmd, args) == -1)
	{
		free_array(new_envp);
		return (-1);
	}
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
	return (0);
}
