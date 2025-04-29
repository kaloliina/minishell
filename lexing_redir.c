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

static void	missing_file_delimiter(t_data *data, t_index *index,
	t_node *new_node)
{
	if (data->tokens[index->i + 1])
		ft_printf(2, SYNTAX, "|");
	else if (new_node->prev && (new_node->prev->type == REDIR_APPEND
			|| new_node->prev->type == REDIR_HEREDOC
			|| new_node->prev->type == REDIR_INF
			|| new_node->prev->type == REDIR_OUTF))
		ft_printf(2, SYNTAX, data->tokens[index->i][index->j]);
	else
		ft_printf(2, SYNTAX, "newline");
}

static int	set_redir_node(t_node *new_node, t_data *data, t_index *index)
{
	set_redir_type(new_node, data, index->i, index->j);
	if (data->tokens[index->i][index->j + 1])
	{
		if (new_node->type == REDIR_HEREDOC)
		{
			new_node->delimiter
				= ft_strdup(data->tokens[index->i][index->j + 1]);
			if (!new_node->delimiter)
				fatal_parsing_exit(data, NULL, NULL, MALLOC);
		}
		else
		{
			new_node->file = ft_strdup(data->tokens[index->i][index->j + 1]);
			if (!new_node->file)
				fatal_parsing_exit(data, NULL, NULL, MALLOC);
		}
	}
	else
	{
		missing_file_delimiter(data, index, new_node);
		free_nodes(data->first);
		return (-1);
	}
	return (0);
}

static int	make_redir_node(t_data *data, int i, int j)
{
	t_node	*new_node;
	t_node	*current;
	t_index	index;

	new_node = NULL;
	new_node = init_new_node(data, new_node);
	index.i = i;
	index.j = j;
	if (!data->first)
		data->first = new_node;
	else
	{
		current = data->first;
		while (current->next)
			current = current->next;
		current->next = new_node;
		new_node->prev = current;
	}
	return (set_redir_node(new_node, data, &index));
}

int	make_all_redir_nodes(t_data *data, int i)
{
	int	j;

	j = 0;
	while (data->tokens[i][j])
	{
		if (is_redirection(data->tokens[i][j]))
		{
			if (make_redir_node(data, i, j) < 0)
				return (-1);
		}
		j++;
	}
	return (0);
}
