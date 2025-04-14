#include "minishell.h"

int	is_redirection(char *token)
{
	if (ft_strcmp(token, ">") && ft_strcmp(token, ">>")
		&& ft_strcmp(token, "<") && ft_strcmp(token, "<<"))
		return (0);
	return (1);
}

static int	make_node(t_data *data, int i, int j, t_node **first)
{
	t_node	*new_node;
	t_node	*current;

	if (is_redirection(data->tokens[i][j]))
		return (j + 2);
	new_node = NULL;
	new_node = init_new_node(data, new_node);
	if (!*first)
		*first = new_node;
	else
	{
		current = *first;
		while (current->next)
			current = current->next;
		current->next = new_node;
		new_node->prev = current;
	}
	return (set_cmd_node(data, i, j, new_node));
}

static void	make_pipe_node(t_data *data, t_node **first)
{
	t_node	*new_node;
	t_node	*current;

	new_node = NULL;
	new_node = init_new_node(data, new_node);
	if (!*first)
		*first = new_node;
	else
	{
		current = *first;
		while (current->next)
			current = current->next;
		current->next = new_node;
		new_node->prev = current;
	}
	new_node->type = PIPE;
}

int	lexer(t_data *data)
{
	int		i;
	int		j;
	int		temp;

	i = 0;
	data->first = NULL;
	while (data->sections[i])
	{
		j = 0;
		if (i > 0)
			make_pipe_node(data, &data->first);
		if (make_all_redir_nodes(data, i) < 0)
			return (-1);
		while (data->tokens[i][j])
			j = make_node(data, i, j, &data->first);
		i++;
	}
	free_sections_tokens(data);
	return (0);
}
