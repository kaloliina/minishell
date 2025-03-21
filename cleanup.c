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

void	free_sections(t_ast *ast, int i)
{
	if (ast->sections[i]->section)
		free (ast->sections[i]->section);
	if (ast->sections[i])
		free (ast->sections[i]);
	i--;
	while (i >= 0)
	{
		free (ast->sections[i]->section);
		free (ast->sections[i]);
		i--;
	}
	free (ast->sections);
}

void	free_struct(t_ast *ast)
{
	int		i;
	t_node	*tmp;

	i = 0;
	while (i < ast->sections_amount)
		free_array(ast->tokens[i++]);
	free (ast->tokens);
	i = 0;
	while (ast->sections[i]->section)
	{
		free (ast->sections[i]->section);
		free (ast->sections[i]);
		i++;
	}
	free (ast->sections[i]);
	free (ast->sections);
	while (ast->first)
	{
		tmp = ast->first;
		ast->first = ast->first->next;
		free (tmp->cmd);
		free (tmp);
	}
}
