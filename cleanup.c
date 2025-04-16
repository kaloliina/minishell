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
	ft_printf(2, "%s\n", msg);
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
	exit (1);
}

void	handle_fatal_exit(char *msg, t_pipes *my_pipes, t_node *list, char *conversion)
{
	if (conversion == NULL)
		ft_printf(2, msg);
	else
		ft_printf(2, msg, conversion);
	if (list == NULL)
		free_nodes(my_pipes->command_node);
	else
		free_nodes(list);
	if (my_pipes != NULL)
	{
		free_array(*my_pipes->my_envp);
		free_my_pipes(my_pipes);
	}
	exit (my_pipes->exit_status);
}
