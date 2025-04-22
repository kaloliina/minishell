#include "minishell.h"

int	export_validation(char **cmd)
{
	int	i;
	int	j;

	i = 1;
	while (cmd[i])
	{
		j = 0;
		if (!(ft_isalpha(cmd[i][j]) || cmd[i][j] == '_'))
		{
			ft_printf(2, EXPORT, cmd[i]);
			return (-1);
		}
		while (cmd[i][j])
		{
			while (!is_exp_delimiter(cmd[i][j]))
				j++;
			if (cmd[i][j] != '=' && cmd[i][j] != '\0')
			{
				ft_printf(2, EXPORT, cmd[i]);
				return (-1);
			}
			j = ft_strlen(cmd[i]);
		}
		i++;
	}
	return (0);
}
