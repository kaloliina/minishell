#include "minishell.h"

static int	exit_is_nonnumeric_arg(char *arg, t_pipes *my_pipes)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!(arg[i] >= 48 && arg[i] <= 57))
		{
			if (!my_pipes->pipe_amount)
				ft_printf(2, "exit\n");
			ft_printf(2, "minishell: %s: %s: %s", "exit", arg, ERR_NUM);
			cleanup_in_exec(my_pipes, NULL);
			return (1);
		}
		i++;
	}
	return (0);
}

void	execute_exit(char **cmd, t_pipes *my_pipes)
{
	int	status;

	status = 0;
	if (cmd[1] && exit_is_nonnumeric_arg(cmd[1], my_pipes))
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
