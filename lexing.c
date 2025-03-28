#include "minishell.h"

static int	set_redir_file_node(t_node *new_node, t_ast *ast, int i, int j)
{
	if (!ft_strcmp(ast->tokens[i][j], "<"))
		new_node->type = REDIR_INF;
	else if (!ft_strcmp(ast->tokens[i][j], ">>"))
		new_node->type = REDIR_APPEND;
	else if (!ft_strcmp(ast->tokens[i][j], ">"))
		new_node->type = REDIR_OUTF;
	else
		new_node->type = REDIR_HEREDOC;
	if (ast->tokens[i][j + 1])
	{
		if (new_node->type == REDIR_HEREDOC)
			new_node->delimiter = ast->tokens[i][j + 1];
		else
			new_node->file = ast->tokens[i][j + 1];
		return (j + 2);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
		if (ast->tokens[i + 1])
			ft_putstr_fd("`|'\n", 2);
		else
			ft_putstr_fd("`newline'\n", 2);
		free_struct(ast);
		return (-1);
	}
	return (j + 2);
}

static int	make_redir_node(t_ast *ast, int i, int j, t_node **first)
{
	t_node	*new_node;
	t_node	*current;

	new_node = NULL;
	new_node = init_new_node(ast, new_node);
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
	return (set_redir_file_node(new_node, ast, i, j));
}

static void	make_all_redir_nodes(t_ast *ast, int i)
{
	int	j;

	j = 0;
	while (ast->tokens[i][j])
	{
		if (is_redirection(ast->tokens[i][j]))
			make_redir_node(ast, i, j, &ast->first);
		j++;
	}
}

int	lexer(t_ast *ast)
{
	int		i;
	int		j;
	int		temp;

	i = 0;
	ast->first = NULL;
	while (ast->sections[i])
	{
		j = 0;
		if (i > 0)
			make_pipe_node(ast, &ast->first);
		make_all_redir_nodes(ast, i);
		while (ast->tokens[i][j])
		{
			temp = make_node(ast, i, j, &ast->first);
			if (temp < 0)
				return (-1);
			j = temp;
		}
		i++;
	}
	return (0);
}
