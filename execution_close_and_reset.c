#include "../minishell.h"

void	close_all_fds(t_pipes *my_pipes)
{
	int	i;

	i = 0;
	while (my_pipes->pipes && i < my_pipes->pipe_amount * 2)
	{
		if (my_pipes->pipes[i] > 0)
		{
			if (close(my_pipes->pipes[i]) < 0)
				ft_printf(2, "minishell: %s", ERR_CLOSE);
			my_pipes->pipes[i] = -1;
		}
		i++;
	}
	if (my_pipes->stdinfd != -1 && (close(my_pipes->stdinfd) < 0))
		ft_printf(2, "minishell: %s", ERR_CLOSE);
	my_pipes->stdinfd = -1;
	if (my_pipes->stdoutfd != -1 && (close(my_pipes->stdoutfd) < 0))
		ft_printf(2, "minishell: %s", ERR_CLOSE);
	my_pipes->stdoutfd = -1;
	if (my_pipes->outfile_fd != -1 && (close(my_pipes->outfile_fd) < 0))
		ft_printf(2, "minishell: %s", ERR_CLOSE);
	my_pipes->outfile_fd = -1;
	if (my_pipes->infile_fd != -1 && (close(my_pipes->infile_fd) < 0))
		ft_printf(2, "minishell: %s", ERR_CLOSE);
	my_pipes->infile_fd = -1;
}

void	free_my_pipes(t_pipes *my_pipes)
{
	if (my_pipes)
	{
		if (my_pipes->paths)
			free_array(my_pipes->paths);
		if (my_pipes->command_path)
		{
			free(my_pipes->command_path);
			my_pipes->command_path = NULL;
		}
		close_all_fds(my_pipes);
		if (my_pipes->pipes)
		{
			free(my_pipes->pipes);
			my_pipes->pipes = NULL;
		}
		if (my_pipes->childpids)
		{
			free(my_pipes->childpids);
			my_pipes->childpids = NULL;
		}
		free(my_pipes);
	}
}

//Double check the if statement here
//I removed && ft_strcmp(my_pipes->command_path, my_pipes->command_node->cmd[0])), double check this
void	reset_properties(t_pipes *my_pipes)
{
	if (my_pipes->command_path != NULL)
	{
		free (my_pipes->command_path);
		my_pipes->command_path = NULL;
	}
	my_pipes->command_node = NULL;
	my_pipes->exit_status = 0;
	if (my_pipes->current_section != (my_pipes->pipe_amount + 1))
	{
		my_pipes->read_end = my_pipes->write_end - 1;
		if (my_pipes->current_section < my_pipes->pipe_amount)
			my_pipes->write_end = my_pipes->write_end + 2;
	}
	my_pipes->current_section++;
}

void	close_pipeline_fds(t_pipes *my_pipes)
{
	if (my_pipes->pipe_amount > 0)
	{
		if (my_pipes->current_section <= my_pipes->pipe_amount)
		{
			if (close(my_pipes->pipes[my_pipes->write_end]) < 0)
				ft_printf(2, "minishell: %s", ERR_CLOSE);
			my_pipes->pipes[my_pipes->write_end] = -1;
		}
		if (my_pipes->current_section != 1)
		{
			if (close(my_pipes->pipes[my_pipes->read_end]) < 0)
				ft_printf(2, "minishell: %s", ERR_CLOSE);
			my_pipes->pipes[my_pipes->read_end] = -1;
		}
	}
	if (my_pipes->outfile_fd != -1 && (close(my_pipes->outfile_fd) < 0))
		ft_printf(2, "minishell: %s", ERR_CLOSE);
	my_pipes->outfile_fd = -1;
	if (my_pipes->infile_fd != -1 && (close(my_pipes->infile_fd) < 0))
		ft_printf(2, "minishell: %s", ERR_CLOSE);
	my_pipes->infile_fd = -1;
	if (my_pipes->current_section != (my_pipes->pipe_amount + 1))
		reset_properties(my_pipes);
}

void	cleanup_in_exec(t_pipes *my_pipes, t_node *list)
{
	if (!list && my_pipes->command_node)
		free_nodes(my_pipes->command_node);
	else if (!list)
		free_nodes(my_pipes->heredoc_node);
	else
		free_nodes(list);
	if (my_pipes)
	{
		if (my_pipes->hd_dir)
			handle_tmpfile(my_pipes);
		free_array(*my_pipes->my_envp);
		free_my_pipes(my_pipes);
	}
}
