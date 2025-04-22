#include "minishell.h"

void	cd_no_args(t_exp *expand, t_pipes *my_pipes)
{
	char	*expansion;

	expansion = find_envp(expand, 0, 0);
	if (!expansion)
	{
		ft_printf(2, "minishell: cd: HOME not set\n");
		my_pipes->exit_status = 1;
	}
	else if (chdir(expansion) == -1)
		perror("minishell");
	free (expansion);
}

void	execute_exit_helper(char **cmd, int *is_num, int *status)
{
	int	i;

	i = 0;
	if (cmd[1][i] == '-' || cmd[1][i] == '+')
		i++;
	while (cmd[1][i] != '\0')
	{
		if (!(cmd[1][i] >= 48 && cmd[1][i] <= 57))
			*is_num = 0;
		i++;
	}
	if (*is_num == 0)
	{
		ft_printf(2, "minishell: %s: %s: %s\n", cmd[0], cmd[1], ERR_NUM);
		*status = 2;
	}
	else
		*status = ft_atoi(cmd[1]);
}
