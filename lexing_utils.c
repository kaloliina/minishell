#include "minishell.h"

static int	count_args(t_ast *ast, int i, int j)
{
	int	args;

	args = 0;
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
	return (args);
}

static int	set_cmd_no_args(t_ast *ast, int i, int init_j, t_node *new_node)
{
	int	k;

	k = 0;
	new_node->cmd = malloc(sizeof(char *) * 2);
	if (!new_node->cmd)
	{
		free_struct(ast);
		ft_printf(2, "minishell: memory allocation failure\n");
		exit (1);
	}
	new_node->cmd[k++] = ast->tokens[i][init_j++];
	new_node->cmd[k] = NULL;
	return (init_j);
}

static int	set_cmd_args(char **token, int init_j, int args, t_node *new_node)
{
	int	k;

	k = 0;
	new_node->cmd[k++] = token[init_j++];
	while (k <= args)
	{
		if (is_redirection(token[init_j]))
			init_j += 2;
		else
		{
			new_node->cmd[k] = token[init_j];
			k++;
			init_j++;
		}
	}
	new_node->cmd[k] = NULL;
	return (init_j);
}

static int	set_cmd_node(t_ast *ast, int i, int j, t_node *new_node)
{
	int	init_j;
	int	args;

	new_node->type = COMMAND;
	init_j = j;
	if (ast->tokens[i][j + 1])
	{
		j++;
		args = count_args(ast, i, j);
		new_node->cmd = malloc(sizeof(char *) * (args + 2));
		if (!new_node->cmd)
		{
			free_struct(ast);
			ft_printf(2, "minishell: memory allocation failure\n");
			exit (1);
		}
		return (set_cmd_args(ast->tokens[i], init_j, args, new_node));
	}
	else
		return (set_cmd_no_args(ast, i, init_j, new_node));
}

int	make_node(t_ast *ast, int i, int j, t_node **first)
{
	t_node	*new_node;
	t_node	*current;

	if (is_redirection(ast->tokens[i][j]))
			return (j + 2);
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
	return (set_cmd_node(ast, i, j, new_node));
}
