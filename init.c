#include "minishell.h"

t_node	*init_new_node(t_data *data, t_node *new_node)
{
	new_node = malloc(sizeof(t_node));
	if (!new_node)
	{
		free_nodes(data->first);
		free_sections_tokens(data);
		//free input
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
	data->sections = ft_split(line, '|'); //malloc protection
	if (!data->sections)
	{
		ft_printf(2, "‰s\n", MALLOC);
		free (line);
		//free input
		exit (1);
	}
	data->sections_amount = count_elements(data->sections);
	free (line);
}

char	**init_only_quotes_section(t_data *data, int i)
{
	data->tokens[i] = malloc(sizeof(char *) * 2);
	//malloc protection
	data->tokens[i][0] = ft_strdup(data->sections[i]);
	//malloc protection
	data->tokens[i][1] = NULL;
	return (data->tokens[i]);
}

void	init_tokens(t_data *data)
{
	int	i;
	int	error;

	data->tokens = malloc(sizeof(char **) * (data->sections_amount + 1));
	if (!data->tokens)
	{
		free_array(data->sections);
		//free input
		ft_printf(2, "%s\n", MALLOC);
		exit (1);
	}
	i = 0;
	while (data->sections[i])
	{
		error = 0;
		if (!is_only_quotes(data->sections[i]))
		{
			data->tokens[i] = ft_ms_split(data->sections[i], ' ', &error);
			if (!data->tokens[i] && error)
			{
				while (--i > 0)
					free_array(data->tokens[i]);
				free_array(data->sections);
				//free input
				ft_printf(2, "%s\n", MALLOC);
				exit (1);
			}
		}
		else
			data->tokens[i] = init_only_quotes_section(data, i);
		i++;
	}
	data->tokens[i] = NULL;
}

void	init_data(t_data *data)
{
	data->first = NULL;
	data->sections = NULL;
	data->sections_amount = 0;
	data->tokens = NULL;
}
