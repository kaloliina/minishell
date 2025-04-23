#include "minishell.h"

t_node	*init_new_node(t_data *data, t_node *new_node)
{
	new_node = malloc(sizeof(t_node));
	if (!new_node)
		fatal_parsing_exit(data, NULL, NULL, MALLOC);
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
	int	error;

	error = 0;
	data->sections = ft_ms_split(line, '|', &error);
	if (!data->sections && error)
		fatal_parsing_exit(data, NULL, line, MALLOC);
	data->sections_amount = count_elements(data->sections);
	free (line);
}

static char	**init_only_quotes_section(t_data *data, int i)
{
	data->tokens[i] = malloc(sizeof(char *) * 2);
	if (!data->tokens[i])
		fatal_parsing_exit(data, NULL, NULL, MALLOC);
	data->tokens[i][0] = ft_strdup(data->sections[i]);
	if (!data->tokens[i][0])
	{
		data->tokens[i + 1] = NULL;
		fatal_parsing_exit(data, NULL, NULL, MALLOC);
	}
	data->tokens[i][1] = NULL;
	return (data->tokens[i]);
}

void	init_tokens(t_data *data)
{
	int	i;
	int	error;

	data->tokens = malloc(sizeof(char **) * (data->sections_amount + 1));
	if (!data->tokens)
		fatal_parsing_exit(data, NULL, NULL, MALLOC);
	i = 0;
	while (data->sections[i])
	{
		if (!is_only_quotes(data->sections[i]))
		{
			data->tokens[i] = ft_ms_split(data->sections[i], ' ', &error);
			if (!data->tokens[i] && error)
				fatal_parsing_exit(data, NULL, NULL, MALLOC);
		}
		else
			data->tokens[i] = init_only_quotes_section(data, i);
		i++;
	}
	data->tokens[i] = NULL;
}

void	init_data(t_data *data, char ***envp)
{
	data->envp = *envp;
	data->first = NULL;
	data->sections = NULL;
	data->sections_amount = 0;
	data->tokens = NULL;
}
