#include "minishell.h"

static int	set_cmd_no_args(t_data *data, t_index *index, t_node *new_node)
{
	int	k;

	k = 0;
	new_node->cmd = malloc(sizeof(char *) * 2);
	if (!new_node->cmd)
		fatal_parsing_exit(data, NULL, NULL, MALLOC);
	new_node->cmd[k++] = ft_strdup(data->tokens[index->i][index->j++]);
	if (!new_node->cmd[k - 1])
		fatal_parsing_exit(data, NULL, NULL, MALLOC);
	new_node->cmd[k] = NULL;
	return (index->j);
}

static int	set_cmd_args(t_data *data, t_index *index,
	int args, t_node *new_node)
{
	int	k;

	k = 0;
	new_node->cmd[k++] = ft_strdup(data->tokens[index->i][index->j++]);
	if (!new_node->cmd[k - 1])
		fatal_parsing_exit(data, NULL, NULL, MALLOC);
	while (k <= args)
	{
		if (is_redirection(data->tokens[index->i][index->j]))
			index->j += 2;
		else
		{
			new_node->cmd[k] = ft_strdup(data->tokens[index->i][index->j]);
			if (!new_node->cmd[k])
				fatal_parsing_exit(data, NULL, NULL, MALLOC);
			k++;
			index->j++;
		}
	}
	new_node->cmd[k] = NULL;
	return (index->j);
}

int	set_cmd_node(t_data *data, t_index *index, t_node *new_node)
{
	int		init_j;
	int		args;
	t_index	index2;

	new_node->type = COMMAND;
	init_j = index->j;
	if (data->tokens[index->i][index->j + 1])
	{
		index->j++;
		args = count_args(data, index->i, index->j);
		new_node->cmd = malloc(sizeof(char *) * (args + 2));
		if (!new_node->cmd)
			fatal_parsing_exit(data, NULL, NULL, MALLOC);
		index2.i = index->i;
		index2.j = init_j;
		return (set_cmd_args(data, &index2, args, new_node));
	}
	else
	{
		index2.i = index->i;
		index2.j = init_j;
		return (set_cmd_no_args(data, &index2, new_node));
	}
}
