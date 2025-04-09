#include "minishell.h"

int	is_redirection(char *token)
{
	if (ft_strcmp(token, ">") && ft_strcmp(token, ">>")
		&& ft_strcmp(token, "<") && ft_strcmp(token, "<<"))
		return (0);
	return (1);
}

int	count_elements(char **sections)
{
	int	i;

	i = 0;
	while (sections[i])
		i++;
	return (i);
}

int	is_quote(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == 34 || s[i] == 39)
			return (1);
		i++;
	}
	return (0);
}

int	is_exp_delimiter(char c)
{
	return (!(ft_isalnum(c) || c == '_'));
}

void	make_pipe_node(t_data *data, t_node **first)
{
	t_node	*new_node;
	t_node	*current;

	new_node = NULL;
	new_node = init_new_node(data, new_node);
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

int	is_only_quotes(char *s)
{
	if (((s[0] == '\'' && s[1] == '\'')
		|| (s[0] == '"' && s[1] == '"'))
		&& s[2] == '\0')
		return (1);
	return (0);
}
