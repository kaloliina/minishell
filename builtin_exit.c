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
				ft_printf(1, "exit\n");
			print_error(ERR_NUM, arg, NULL);
			cleanup_in_exec(my_pipes, NULL);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	ft_atoi_ms(const char *string, int *res)
{
	int			i;
	int			sign;
	long long	nbr;

	i = 0;
	sign = 1;
	nbr = 0;
	if (string[i] == '+' || string[i] == '-')
	{
		if (string[i] == '-')
			sign = -sign;
		i++;
	}
	while (string[i] <= '9' && string[i] >= '0')
	{
		nbr = nbr * 10 + (string[i] - '0');
		if ((nbr * sign) > INT_MAX || (nbr * sign) < INT_MIN)
			return (1);
		i++;
	}
	*res = nbr * sign;
	return (0);
}

static int	exit_conversion(char *arg, t_pipes *my_pipes, int *error)
{
	int	invalid_nbr;
	int	status;

	status = 0;
	invalid_nbr = ft_atoi_ms(arg, &status);
	if (!invalid_nbr)
		return (status);
	*error = 1;
	if (!my_pipes->pipe_amount)
		ft_printf(1, "exit\n");
	print_error(ERR_NUM, arg, NULL);
	return (2);
}

void	execute_exit(char **cmd, t_pipes *my_pipes)
{
	int	status;
	int	error;

	status = 0;
	error = 0;
	if (cmd[1] && exit_is_nonnumeric_arg(cmd[1], my_pipes))
		exit (2);
	if (count_elements(cmd) > 2)
	{
		if (!my_pipes->pipe_amount)
			ft_printf(1, "exit\n");
		print_error("exit: %s", ERR_ARG, NULL);
		my_pipes->exit_status = 1;
		if (my_pipes->pipe_amount)
			fatal_exec_error(NULL, my_pipes, NULL, NULL);
		return ;
	}
	if (cmd[1])
		status = exit_conversion(cmd[1], my_pipes, &error);
	if (!my_pipes->pipe_amount && !error)
		ft_printf(1, "exit\n");
	cleanup_in_exec(my_pipes, NULL);
	exit ((unsigned int) status);
}
