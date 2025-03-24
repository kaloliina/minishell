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

void	free_struct(t_ast *ast)
{
	int		i;
	// int		j;
	t_node	*tmp;

	i = 0;
	while (ast->sections[i])
		free (ast->sections[i++]);
	free (ast->sections);
	while (ast->first)
	{
		tmp = ast->first;
		ast->first = ast->first->next;
		free_array(tmp->cmd);
		if (tmp->file)
			free (tmp->file);
		free (tmp);
	}
	i = 0;
	while (ast->tokens[i])
		free_array(ast->tokens[i++]);
	free (ast->tokens);
}
