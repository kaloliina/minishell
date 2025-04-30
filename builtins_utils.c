#include "minishell.h"

void	cd_no_args(t_exp *expand, t_pipes *my_pipes)
{
	char	*expansion;

	expand->exp = ft_strdup("HOME");
	if (!expand->exp)
		handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
	expansion = find_envp(expand, 0);
	if (!expansion)
	{
		ft_printf(2, "minishell: cd: HOME not set\n");
		my_pipes->exit_status = 1;
	}
	else if (chdir(expansion) == -1)
		perror("minishell: cd");
	free (expansion);
	free (expand->exp);
}

int	exit_is_nonnumeric_arg(char *arg, t_pipes *my_pipes)
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
