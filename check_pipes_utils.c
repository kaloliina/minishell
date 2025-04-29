#include "minishell.h"

void	check_for_ctrld(char *temp, t_data *data, char *line)
{
	if (!temp)
	{
		ft_printf(2, "minishell: ");
		ft_printf(2, ERR_EOF);
		ft_printf(2, "exit\n");
		fatal_parsing_exit(data, NULL, line, NULL);
	}
}

int	end_pipe_sigint(int backup_fd, char *temp, char *line, int *status)
{
	if (g_signum == SIGINT)
	{
		*status = g_signum + 128;
		g_signum = 0;
		dup2(backup_fd, STDIN_FILENO);
		close(backup_fd);
		free (temp);
		free (line);
		return (1);
	}
	return (0);
}
