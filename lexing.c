#include "minishell.h"

/*IF THERE IS REDIR AT ANY POINT OF TOKEN, CUT THE TOKEN THERE AND MAKE A NEW ONE!!!
TAKE OUT THESE END/START SEPARATE HANDLING CASES
REDIR CHARS CANNOT BE PART OF FILENAME, SO ONLY HANDLE QUOTES (INSIDE QUOTES REDIR DOESNT CUT)*/

int	set_cmd_node(t_ast *ast, int i, int j, t_node *new_node)
{
	int	init_j;
	int	k;
	int	args;

	new_node->type = COMMAND;
	args = 0;
	init_j = j;
	if (ast->tokens[i][j + 1] && !is_redirection(ast->tokens[i][j + 1]))
	{
		j++;
		while (ast->tokens[i][j])
		{
			if (is_redirection(ast->tokens[i][j]))
				j += 2;
			else
			{
				args++;
				j++;
			}
		}
		new_node->cmd = malloc(sizeof(char *) * (args + 2));
		if (!new_node->cmd)
		{
			free_struct(ast);
			ft_putstr_fd("minishell: memory allocation failure\n", 2);
			exit (1);
		}
		k = 0;
		new_node->cmd[k] = ast->tokens[i][init_j];
		k++;
		init_j++;
		while (k <= args)
		{
			new_node->cmd[k] = ast->tokens[i][init_j];
			k++;
			init_j++;
		}
		new_node->cmd[k] = NULL;
	}
	else
	{
		k = 0;
		new_node->cmd = malloc(sizeof(char *) * 2);
		if (!new_node->cmd)
		{
			free_struct(ast);
			ft_putstr_fd("minishell: memory allocation failure\n", 2);
			exit (1);
		}
		new_node->cmd[k++] = ast->tokens[i][init_j++];
		new_node->cmd[k] = NULL;
	}
	return (init_j);
}

int	set_redir_file_node(t_node *new_node, t_ast *ast, int i, int j)
{
	if (!ft_strcmp(ast->tokens[i][j], "<"))
		new_node->type = REDIR_INF;
	else if (!ft_strcmp(ast->tokens[i][j], ">>"))
		new_node->type = REDIR_APPEND;
	else
		new_node->type = REDIR_OUTF;
	if (ast->tokens[i][j + 1])
		new_node->file = ast->tokens[i][j + 1];
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

int	set_node(t_node *new_node, t_ast *ast, int i, int j)
{
	if (!ft_strcmp(ast->tokens[i][j], "<")
		|| !ft_strcmp(ast->tokens[i][j], ">")
		|| !ft_strcmp(ast->tokens[i][j], ">>"))
		return (set_redir_file_node(new_node, ast, i, j));
	else if (!ft_strcmp(ast->tokens[i][j], "<<"))
		new_node->type = REDIR_HEREDOC;
	else
		return (set_cmd_node(ast, i, j, new_node));
	return (j + 1);
}

int	make_node(t_ast *ast, int i, int j, t_node **first)
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
	return (set_node(new_node, ast, i, j));
}

int	lexer(t_ast *ast)
{
	int		i;
	int		j;
	int		temp;

	i = 0;
	ast->first = NULL;
	while (ast->sections[i]->section)
	{
		j = 0;
		if (i > 0)
			make_pipe_node(ast, &ast->first);
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
