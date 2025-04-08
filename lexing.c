#include "minishell.h"

static int	set_redir_file_node(t_node *new_node, t_data *data, int i, int j)
{
	if (!ft_strcmp(data->tokens[i][j], "<"))
		new_node->type = REDIR_INF;
	else if (!ft_strcmp(data->tokens[i][j], ">>"))
		new_node->type = REDIR_APPEND;
	else if (!ft_strcmp(data->tokens[i][j], ">"))
		new_node->type = REDIR_OUTF;
	else
		new_node->type = REDIR_HEREDOC;
	if (data->tokens[i][j + 1])
	{
		if (new_node->type == REDIR_HEREDOC)
			new_node->delimiter = ft_strdup(data->tokens[i][j + 1]);
		else
			new_node->file = ft_strdup(data->tokens[i][j + 1]);
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
	return (j + 2);
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
	return (set_redir_file_node(new_node, data, i, j));
}

static void	make_all_redir_nodes(t_data *data, int i)
{
	int	j;

	j = 0;
	while (data->tokens[i][j])
	{
		if (is_redirection(data->tokens[i][j]))
			make_redir_node(data, i, j, &data->first);
		j++;
	}
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
		make_all_redir_nodes(data, i);
		while (data->tokens[i][j])
		{
			temp = make_node(data, i, j, &data->first);
			if (temp < 0)
				return (-1);
			j = temp;
		}
		i++;
	}
	free_sections_tokens(data);
	return (0);
}
