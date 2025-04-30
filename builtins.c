#include "minishell.h"

void	execute_echo(t_node *node)
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
	t_exp	expand;

	init_exp(&expand, 0, NULL, my_pipes);
	if (count_elements(cmd) == 1)
		cd_no_args(&expand, my_pipes);
	else if (count_elements(cmd) > 2)
	{
		ft_printf(2, "minishell: %s: %s", cmd[0], ERR_ARG);
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
}

void	execute_exit(char **cmd, t_pipes *my_pipes)
{
	int	status;

	status = 0;
	if (exit_is_nonnumeric_arg(cmd[1], my_pipes))
		exit (2);
	if (count_elements(cmd) > 2)
	{
		if (!my_pipes->pipe_amount)
			ft_printf(2, "exit\n");
		ft_printf(2, "minishell: %s: %s", cmd[0], ERR_ARG);
		if (my_pipes->pipe_amount)
		{
			cleanup_in_exec(my_pipes, NULL);
			exit (1);
		}
		my_pipes->exit_status = 1;
		return ;
	}
	if (cmd[1] != NULL)
		status = ft_atoi(cmd[1]);
	if (!my_pipes->pipe_amount)
		ft_printf(1, "exit\n");
	cleanup_in_exec(my_pipes, NULL);
	exit ((unsigned int) status);
}
