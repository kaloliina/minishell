#include "minishell.h"

void	fatal_export_unset_error(char **new_envp, t_pipes *my_pipes)
{
	free_array(new_envp);
	handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
}

void	free_envp_array(char **envp, int i)
{
	int	j;

	j = 0;
	if (envp)
	{
		while (envp[j])
		{
			if (envp[j])
				free (envp[j++]);
		}
		if (j == i)
			j++;
		while (envp[j])
		{
			if (envp[j])
				free (envp[j++]);
		}
	}
}

void	fatal_pwd_error(char *msg, t_pipes *my_pipes, int i)
{
	int	exit_status;

	exit_status = 1;
	if (msg)
		ft_printf(2, "minishell: %s", msg);
	if (my_pipes->command_node)
		free_nodes(my_pipes->command_node);
	else
		free_nodes(my_pipes->heredoc_node);
	if (my_pipes)
	{
		exit_status = my_pipes->exit_status;
		if (my_pipes->hd_dir)
			handle_tmpfile(my_pipes);
		free_envp_array(*my_pipes->my_envp, i);
		free_my_pipes(my_pipes);
	}
	exit (exit_status);
}
