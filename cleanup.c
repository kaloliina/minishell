#include "minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (array)
	{
		while (array[i])
			free (array[i++]);
		free (array);
	}
}

void	free_struct(t_data *data)
{
	int		i;
	t_node	*tmp;

	while (data->first)
	{
		tmp = data->first;
		data->first = data->first->next;
		free_array(tmp->cmd);
		if (tmp->file)
			free (tmp->file);
		if (tmp->delimiter)
			free (tmp->delimiter);
		free (tmp);
	}
}

void	free_sections_tokens(t_data *data)
{
	int	i;

	i = 0;
	while (data->sections[i])
		free (data->sections[i++]);
	free (data->sections);
	i = 0;
	while (data->tokens[i])
		free_array(data->tokens[i++]);
	free (data->tokens);
}
