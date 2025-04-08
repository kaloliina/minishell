#include "minishell.h"

t_node	*init_new_node(t_data *data, t_node *new_node)
{
	new_node = malloc(sizeof(t_node));
	if (!new_node)
	{
		free_struct(data);
		ft_printf(2, "%s\n", MALLOC);
		exit (1);
	}
	new_node->cmd = NULL;
	new_node->file = NULL;
	new_node->delimiter = NULL;
	new_node->delimiter_quote = 0;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	init_sections(t_data *data, char *line)
{
	data->sections = ft_split(line, '|');
	if (!data->sections)
		return ;
	data->sections_amount = count_elements(data->sections);
	free (line);
}

void	init_tokens(t_data *data)
{
	int	i;
	int	error;

	data->tokens = malloc(sizeof(char **) * (data->sections_amount + 1));
	if (!data->tokens)
	{
		free_array(data->sections);
		ft_printf(2, "%s\n", MALLOC);
		exit (1);
	}
	i = 0;
	while (data->sections[i])
	{
		error = 0;
		data->tokens[i] = ft_ms_split(data->sections[i], ' ', &error);
		if (!data->tokens[i] && error)
		{
			free_struct(data);
			ft_printf(2, "%s\n", MALLOC);
			exit (1);
		}
		i++;
	}
	data->tokens[i] = NULL;
}

void	init_tokens_struct(t_data *data)
{
	data->first = NULL;
	data->sections = NULL;
	data->sections_amount = 0;
	data->tokens = NULL;
}
