#include "minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (array)
	{
		while (array[i])
			free (array[i++]);
		free (array);
	}
}

void	free_nodes(t_node *node)
{
	t_node	*tmp;

	while (node->prev)
		node = node->prev;
	while (node)
	{
		tmp = node;
		node = node->next;
		free_array(tmp->cmd);
		if (tmp->file)
			free (tmp->file);
		if (tmp->delimiter)
			free (tmp->delimiter);
		if (tmp->hd_fd != -1)
		{
			close (tmp->hd_fd);
			tmp->hd_fd = -1;
		}
		free (tmp);
	}
}

void	free_sections_tokens(t_data *data)
{
	int	i;

	i = 0;
	if (data->sections)
	{
		while (data->sections[i])
			free (data->sections[i++]);
		free (data->sections);
		data->sections = NULL;
	}
	i = 0;
	if (data->tokens)
	{
		while (data->tokens[i])
			free_array(data->tokens[i++]);
		free (data->tokens);
		data->tokens = NULL;
	}
}

void	fatal_parsing_exit(t_data *data, t_exp *expand, char *input, char *msg)
{
	if (msg)
		ft_printf(2, "%s", msg);
	if (input)
		free (input);
	if (data)
	{
		free_array(data->envp);
		free_sections_tokens(data);
		if (data->first)
			free_nodes(data->first);
	}
	if (expand)
	{
		if (expand->my_pipes)
			free_my_pipes(expand->my_pipes);
		if (expand->new_cmd)
			free_array(expand->new_cmd);
		if (expand->exp)
			free (expand->exp);
	}
	if (!msg)
		exit (2);
	exit (1);
}

void	handle_fatal_exit(char *msg, t_pipes *my_pipes, t_node *list,
	char *conversion)
{
	int	exit_status;

	exit_status = 1;
	if (!conversion)
		ft_printf(2, msg);
	else
		ft_printf(2, msg, conversion);
	if (!list && my_pipes->command_node)
		free_nodes(my_pipes->command_node);
	else if (!list)
		free_nodes(my_pipes->heredoc_node);
	else
		free_nodes(list);
	if (my_pipes)
	{
		exit_status = my_pipes->exit_status;
		if (my_pipes->hd_dir)
			handle_tmpfile(my_pipes);
		free_array(*my_pipes->my_envp);
		free_my_pipes(my_pipes);
	}
	exit (exit_status);
}
