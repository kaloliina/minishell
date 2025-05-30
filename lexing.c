/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:11:58 by sojala            #+#    #+#             */
/*   Updated: 2025/05/04 18:12:00 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(t_data *parser, int i, int j)
{
	int	args;

	args = 0;
	while (parser->tokens[i][j])
	{
		if (is_redirection(parser->tokens[i][j]))
			j += 2;
		else
		{
			args++;
			j++;
		}
	}
	return (args);
}

int	is_redirection(char *token)
{
	if (ft_strcmp(token, ">") && ft_strcmp(token, ">>")
		&& ft_strcmp(token, "<") && ft_strcmp(token, "<<"))
		return (0);
	return (1);
}

static int	make_node(t_data *parser, int i, int j)
{
	t_node	*new_node;
	t_node	*current;
	t_index	index;

	if (is_redirection(parser->tokens[i][j]))
		return (j + 2);
	index.i = i;
	index.j = j;
	new_node = NULL;
	new_node = init_new_node(parser, new_node);
	if (!parser->first)
		parser->first = new_node;
	else
	{
		current = parser->first;
		while (current->next)
			current = current->next;
		current->next = new_node;
		new_node->prev = current;
	}
	return (set_cmd_node(parser, &index, new_node));
}

static void	make_pipe_node(t_data *parser, t_node **first)
{
	t_node	*new_node;
	t_node	*current;

	new_node = NULL;
	new_node = init_new_node(parser, new_node);
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

int	lexer(t_data *parser)
{
	int		i;
	int		j;

	i = 0;
	parser->first = NULL;
	while (parser->sections[i])
	{
		j = 0;
		if (i > 0)
			make_pipe_node(parser, &parser->first);
		if (make_all_redir_nodes(parser, i) < 0)
		{
			free_sections_tokens(parser);
			return (-1);
		}
		while (parser->tokens[i][j])
			j = make_node(parser, i, j);
		i++;
	}
	free_sections_tokens(parser);
	return (0);
}
