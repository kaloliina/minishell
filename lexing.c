#include "minishell.h"

static void	set_redir_type(t_node *new_node, t_data *data, int i, int j)
{
	if (!ft_strcmp(data->tokens[i][j], "<"))
		new_node->type = REDIR_INF;
	else if (!ft_strcmp(data->tokens[i][j], ">>"))
		new_node->type = REDIR_APPEND;
	else if (!ft_strcmp(data->tokens[i][j], ">"))
		new_node->type = REDIR_OUTF;
	else
		new_node->type = REDIR_HEREDOC;
}

static int	set_redir_node(t_node *new_node, t_data *data, int i, int j)
{
	set_redir_type(new_node, data, i, j);
	if (data->tokens[i][j + 1])
	{
		if (new_node->type == REDIR_HEREDOC)
			new_node->delimiter = ft_strdup(data->tokens[i][j + 1]); //malloc protection
		else
			new_node->file = ft_strdup(data->tokens[i][j + 1]); //malloc protection
	}
	else
	{
		ft_printf(2, "minishell: syntax error near unexpected token ");
		if (data->tokens[i + 1])
			ft_printf(2, "`|'\n");
		else
			ft_printf(2, "`newline'\n");
		free_nodes(data->first);
		return (-1);
	}
	return (0);
}

static int	make_redir_node(t_data *data, int i, int j, t_node **first)
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
	return (set_redir_node(new_node, data, i, j));
}

static int	make_all_redir_nodes(t_data *data, int i)
{
	int	j;

	j = 0;
	while (data->tokens[i][j])
	{
		if (is_redirection(data->tokens[i][j]))
		{
			if (make_redir_node(data, i, j, &data->first) < 0)
				return (-1);
		}
		j++;
	}
	return (0);
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
