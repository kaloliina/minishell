/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:11:54 by sojala            #+#    #+#             */
/*   Updated: 2025/05/04 18:11:55 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_redir_type(t_node *new_node, t_data *parser, int i, int j)
{
	if (!ft_strcmp(parser->tokens[i][j], "<"))
		new_node->type = REDIR_INF;
	else if (!ft_strcmp(parser->tokens[i][j], ">>"))
		new_node->type = REDIR_APPEND;
	else if (!ft_strcmp(parser->tokens[i][j], ">"))
		new_node->type = REDIR_OUTF;
	else
		new_node->type = REDIR_HEREDOC;
}

static void	missing_file_delimiter(t_data *parser, t_index *index,
	t_node *new_node)
{
	if (parser->tokens[index->i + 1])
		print_error(ERR_SYNTAX, "|", NULL);
	else if (new_node->prev && (new_node->prev->type == REDIR_APPEND
			|| new_node->prev->type == REDIR_HEREDOC
			|| new_node->prev->type == REDIR_INF
			|| new_node->prev->type == REDIR_OUTF))
		print_error(ERR_SYNTAX, parser->tokens[index->i][index->j], NULL);
	else
		print_error(ERR_SYNTAX, "newline", NULL);
}

static int	set_redir_node(t_node *new_node, t_data *parser, t_index *index)
{
	set_redir_type(new_node, parser, index->i, index->j);
	if (parser->tokens[index->i][index->j + 1])
	{
		if (new_node->type == REDIR_HEREDOC)
		{
			new_node->delimiter
				= ft_strdup(parser->tokens[index->i][index->j + 1]);
			if (!new_node->delimiter)
				fatal_parsing_error(parser, NULL, NULL, ERR_MALLOC);
		}
		else
		{
			new_node->file = ft_strdup(parser->tokens[index->i][index->j + 1]);
			if (!new_node->file)
				fatal_parsing_error(parser, NULL, NULL, ERR_MALLOC);
		}
	}
	else
	{
		missing_file_delimiter(parser, index, new_node);
		free_nodes(parser->first);
		return (-1);
	}
	return (0);
}

static int	make_redir_node(t_data *parser, int i, int j)
{
	t_node	*new_node;
	t_node	*current;
	t_index	index;

	new_node = NULL;
	new_node = init_new_node(parser, new_node);
	index.i = i;
	index.j = j;
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
	return (set_redir_node(new_node, parser, &index));
}

int	make_all_redir_nodes(t_data *parser, int i)
{
	int	j;

	j = 0;
	while (parser->tokens[i][j])
	{
		if (is_redirection(parser->tokens[i][j]))
		{
			if (make_redir_node(parser, i, j) < 0)
				return (-1);
		}
		j++;
	}
	return (0);
}
