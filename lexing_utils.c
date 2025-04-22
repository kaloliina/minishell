#include "minishell.h"

int	count_args(t_data *data, int i, int j)
{
	int	args;

	args = 0;
	while (data->tokens[i][j])
	{
		if (is_redirection(data->tokens[i][j]))
			j += 2;
		else
		{
			args++;
			j++;
		}
	}
	return (args);
}
