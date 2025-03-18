#include "minishell.h"

t_node	*init_new_node(t_ast *ast, t_node *new_node)
{
	new_node = malloc(sizeof(t_node));
	if (!new_node)
	{
		free_struct(ast);
		ft_putstr_fd("minishell: memory allocation failure\n", 2);
		exit (1);
	}
	new_node->file = NULL;
	new_node->cmd = NULL;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	init_sections(t_ast *ast, char *line)
{
	char	**tmp_sections;
	int		i;

	i = 0;
	tmp_sections = ft_split(line, '|');
	if (!tmp_sections)
		return ;
	free (line);
	ast->sections_amount = count_elements(tmp_sections);
	ast->sections = malloc(sizeof(t_section *) * (ast->sections_amount + 1));
	if (!ast->sections)
	{
		free_array(tmp_sections);
		ft_putstr_fd("minishell: memory allocation failure\n", 2);
		exit (1);
	}
	i = set_sections(ast, tmp_sections);
	set_last_section(ast, i, tmp_sections);
	free_array(tmp_sections);
}

void	init_tokens(t_ast *ast)
{
	int	i;
	int	error;

	ast->tokens = malloc(sizeof(char **) * (ast->sections_amount + 1));
	if (!ast->tokens)
	{
		free_sections(ast, ast->sections_amount - 1);
		ft_putstr_fd("minishell: memory allocation failure\n", 2);
		exit (1);
	}
	i = 0;
	while (i < ast->sections_amount)
	{
		error = 0;
		ast->tokens[i] = ft_ms_split(ast->sections[i]->section, ' ', &error);
		if (!ast->tokens[i] && error)
		{
			free_struct(ast);
			ft_putstr_fd("minishell: memory allocation failure\n", 2);
			exit (1);
		}
		i++;
	}
	ast->tokens[i] = NULL;
}

int	set_sections(t_ast *ast, char **tmp_sections)
{
	int	i;

	i = 0;
	while (tmp_sections[i])
	{
		ast->sections[i] = malloc(sizeof(t_section));
		if (!ast->sections[i])
		{
			free_sections(ast, i);
			free_array(tmp_sections);
			ft_putstr_fd("minishell: memory allocation failure\n", 2);
			exit (1);
		}
		ast->sections[i]->section = ft_strdup(tmp_sections[i]);
		if (!ast->sections[i]->section)
		{
			free_sections(ast, i);
			free_array(tmp_sections);
			ft_putstr_fd("minishell: memory allocation failure\n", 2);
			exit (1);
		}
		ast->sections[i]->number = i;
		i++;
	}
	return (i);
}

void	set_last_section(t_ast *ast, int i, char **tmp_sections)
{
	ast->sections[i] = malloc(sizeof(t_section));
	if (!ast->sections[i])
	{
		free_sections(ast, i);
		free_array(tmp_sections);
		ft_putstr_fd("minishell: memory allocation failure\n", 2);
		exit (1);
	}
	ast->sections[i]->section = NULL;
	ast->sections[i]->number = i;
}
