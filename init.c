#include "minishell.h"

t_node	*init_new_node(t_ast *ast, t_node *new_node)
{
	new_node = malloc(sizeof(t_node));
	if (!new_node)
	{
		free_struct(ast);
		ft_printf(2, "minishell: memory allocation failure\n");
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

void	init_sections(t_ast *ast, char *line)
{
	ast->sections = ft_split(line, '|');
	if (!ast->sections)
		return ;
	ast->sections_amount = count_elements(ast->sections);
	free (line);
}

void	init_tokens(t_ast *ast)
{
	int	i;
	int	error;

	ast->tokens = malloc(sizeof(char **) * (ast->sections_amount + 1));
	if (!ast->tokens)
	{
		free_array(ast->sections);
		ft_printf(2, "minishell: memory allocation failure\n");
		exit (1);
	}
	i = 0;
	while (ast->sections[i])
	{
		error = 0;
		ast->tokens[i] = ft_ms_split(ast->sections[i], ' ', &error);
		if (!ast->tokens[i] && error)
		{
			free_struct(ast);
			ft_printf(2, "minishell: memory allocation failure\n");
			exit (1);
		}
		i++;
	}
	ast->tokens[i] = NULL;
}

void	init_tokens_struct(t_ast *ast)
{
	ast->first = NULL;
	ast->sections = NULL;
	ast->sections_amount = 0;
	ast->tokens = NULL;
}
