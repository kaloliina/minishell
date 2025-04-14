#include "minishell.h"

void	make_pipe_node(t_data *data, t_node **first)
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

static int	count_args(t_data *data, int i, int j)
{
	int	args;

	args = 0;
	while (data->tokens[i][j])
	{
		if (is_redirection(data->tokens[i][j]))
			j += 2;
		else
		{
			args++;
			j++;
		}
	}
	return (args);
}

static int	set_cmd_no_args(t_data *data, int i, int init_j, t_node *new_node)
{
	int	k;

	k = 0;
	new_node->cmd = malloc(sizeof(char *) * 2);
	if (!new_node->cmd)
	{
		free_nodes(data->first);
		ft_printf(2, "%s\n", MALLOC);
		exit (1);
	}
	new_node->cmd[k++] = ft_strdup(data->tokens[i][init_j++]);
	//malloc protection
	new_node->cmd[k] = NULL;
	return (init_j);
}

static int	set_cmd_args(char **token, int init_j, int args, t_node *new_node)
{
	int	k;

	k = 0;
	new_node->cmd[k++] = ft_strdup(token[init_j++]);
	//malloc protection
	while (k <= args)
	{
		if (is_redirection(token[init_j]))
			init_j += 2;
		else
		{
			new_node->cmd[k] = ft_strdup(token[init_j]);
			//malloc protection
			k++;
			init_j++;
		}
	}
	new_node->cmd[k] = NULL;
	return (init_j);
}

static int	set_cmd_node(t_data *data, int i, int j, t_node *new_node)
{
	int	init_j;
	int	args;

	new_node->type = COMMAND;
	init_j = j;
	if (data->tokens[i][j + 1])
	{
		j++;
		args = count_args(data, i, j);
		new_node->cmd = malloc(sizeof(char *) * (args + 2));
		if (!new_node->cmd)
		{
			free_nodes(data->first);
			ft_printf(2, "%s\n", MALLOC);
			exit (1);
		}
		return (set_cmd_args(data->tokens[i], init_j, args, new_node));
	}
	else
		return (set_cmd_no_args(data, i, init_j, new_node));
}

int	make_node(t_data *data, int i, int j, t_node **first)
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
