/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:11:50 by sojala            #+#    #+#             */
/*   Updated: 2025/05/04 18:11:51 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_cmd_no_args(t_data *parser, t_index *index, t_node *new_node)
{
	int	k;

	k = 0;
	new_node->cmd = ft_calloc(2, sizeof(char *));
	if (!new_node->cmd)
		fatal_parsing_error(parser, NULL, NULL, ERR_MALLOC);
	new_node->cmd[k++] = ft_strdup(parser->tokens[index->i][index->j++]);
	if (!new_node->cmd[k - 1])
		fatal_parsing_error(parser, NULL, NULL, ERR_MALLOC);
	new_node->cmd[k] = NULL;
	return (index->j);
}

static int	set_cmd_args(t_data *parser, t_index *index,
	int args, t_node *new_node)
{
	int	k;

	k = 0;
	new_node->cmd[k++] = ft_strdup(parser->tokens[index->i][index->j++]);
	if (!new_node->cmd[k - 1])
		fatal_parsing_error(parser, NULL, NULL, ERR_MALLOC);
	while (k <= args)
	{
		if (is_redirection(parser->tokens[index->i][index->j]))
			index->j += 2;
		else
		{
			new_node->cmd[k] = ft_strdup(parser->tokens[index->i][index->j]);
			if (!new_node->cmd[k])
				fatal_parsing_error(parser, NULL, NULL, ERR_MALLOC);
			k++;
			index->j++;
		}
	}
	new_node->cmd[k] = NULL;
	return (index->j);
}

int	set_cmd_node(t_data *parser, t_index *index, t_node *new_node)
{
	int		init_j;
	int		args;
	t_index	index2;

	new_node->type = COMMAND;
	init_j = index->j;
	if (parser->tokens[index->i][index->j + 1])
	{
		index->j++;
		args = count_args(parser, index->i, index->j);
		new_node->cmd = ft_calloc((args + 2), sizeof(char *));
		if (!new_node->cmd)
			fatal_parsing_error(parser, NULL, NULL, ERR_MALLOC);
		index2.i = index->i;
		index2.j = init_j;
		return (set_cmd_args(parser, &index2, args, new_node));
	}
	else
	{
		index2.i = index->i;
		index2.j = init_j;
		return (set_cmd_no_args(parser, &index2, new_node));
	}
}
