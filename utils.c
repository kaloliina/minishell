#include "minishell.h"

int	is_redirection(char *token)
{
	if (ft_strcmp(token, ">") && ft_strcmp(token, ">>")
		&& ft_strcmp(token, "<") && ft_strcmp(token, "<<"))
		return (0);
	return (1);
}

int	count_elements(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

void	make_pipe_node(t_ast *ast, t_node **first)
{
	t_node	*new_node;
	t_node	*current;

	new_node = NULL;
	new_node = init_new_node(ast, new_node);
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

int	is_char_redirection(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

int	is_redirection_char(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '<' || s[i] == '>')
			return (1);
		i++;
	}
	return (0);
}
