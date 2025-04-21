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

void	execute_pwd(t_pipes *my_pipes)
{
	char	*buf;

	buf = malloc(4096);
	if (!buf)
		handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
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
		{
			ft_printf(2, "minishell: cd: HOME not set\n");
			my_pipes->exit_status = 1;
		}
		else if (chdir(expansion) == -1)
			perror("minishell");
		free (expansion);
	}
	if (count_elements(cmd) > 2)
	{
		ft_printf(2, "minishell: %s: %s\n", cmd[0], ERR_ARG);
		my_pipes->exit_status = 1;
	}
	else if (count_elements(cmd) == 2)
	{
		if (chdir(cmd[1]) == -1)
		{
			ft_printf(2, "minishell: %s: %s: ", cmd[0], cmd[1]);
			perror("");
			my_pipes->exit_status = 1;
		}
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
	if (cmd[2])
	{
		ft_printf(2, "minishell: %s: %s\n", cmd[0], ERR_ARG);
		status = 1;
	}
	else if (cmd[1] != NULL)
	{
		if (cmd[1][i] == '-' || cmd[1][i] == '+')
			i++;
		while (cmd[1][i] != '\0')
		{
			if (!(cmd[1][i] >= 48 && cmd[1][i] <= 57))
				is_num = 0;
			i++;
		}
		if (is_num == 0)
		{
			ft_printf(2, "minishell: %s: %s: %s\n", cmd[0], cmd[1], ERR_NUM);
			status = 2;
		}
		else
			status = ft_atoi(cmd[1]);
	}
	if (is_num && !cmd[2])
		printf("exit\n");
	free_array(*my_pipes->my_envp);
	free_nodes(my_pipes->command_node);
	free_my_pipes(my_pipes);
	exit (status);
}
