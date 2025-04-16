#include "minishell.h"

void	execute_echo(t_node *node, char ***envp)
{
	int		i;

	i = 1;
	if (!node->cmd[i])
	{
		ft_printf(1, "\n");
		return ;
	}
	if (node->cmd[i] && !ft_strcmp(node->cmd[i], "-n"))
		i++;
	while (node->cmd[i])
	{
		ft_printf(1, "%s", node->cmd[i]);
		if (node->cmd[i + 1])
			ft_printf(1, " ");
		i++;
	}
	if (ft_strcmp(node->cmd[1], "-n"))
		ft_printf(1, "\n");
}

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

void	execute_pwd(void)
{
	char	*buf;

	buf = malloc(4096);
	if (!buf)
	{
		ft_printf(2, "%s\n", MALLOC); //must free my_pipes and nodes
		exit (1);
	}
	getcwd(buf, 4096);
	if (!buf)
		perror("minishell");
	else
		ft_printf(1, "%s\n", buf);
	free (buf);
}

void	execute_cd(char **cmd, t_pipes *my_pipes)
{
	char	*expansion;
	t_exp	expand;

	init_exp(&expand, 0, NULL, my_pipes);
	expand.exp = ft_strdup("HOME");
	if (!expand.exp)
		handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
	if (count_elements(cmd) == 1)
	{
		expansion = find_envp(&expand, 0, 0);
		if (!expansion)
			ft_printf(2, "minishell: cd: HOME not set\n");
		if (chdir(expansion) == -1)
			perror("minishell");
		free (expansion);
	}
	if (count_elements(cmd) > 2)
		ft_printf(2, "minishell: cd: too many arguments\n");
	else if (count_elements(cmd) == 2)
	{
		if (chdir(cmd[1]) == -1)
			perror("minishell");
	}
	free (expand.exp);
}

void	execute_exit(char **cmd, t_pipes *my_pipes)
{
	int	status;
	int	i;
	int	is_num;

	status = 0;
	is_num = 1;
	i = 0;
	if (cmd[1] != NULL)
	{
		while (cmd[1][i] != '\0')
		{
			if (!(cmd[1][i] >= 48 && cmd[1][i] <= 57))
				is_num = 0;
			i++;
		}
		if (is_num == 0)
		{
			ft_printf(2, "%s: %s: %s\n", cmd[0], cmd[1], ERR_NUM);
			status = 2;
		}
		else
			status = ft_atoi(cmd[1]);
	}
	printf("exit status: %d\n", status);
	free_array(*my_pipes->my_envp);
	free_my_pipes(my_pipes);
	exit (status);
}
