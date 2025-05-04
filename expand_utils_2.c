/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:11:15 by sojala            #+#    #+#             */
/*   Updated: 2025/05/04 18:11:16 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count_expandable(char *arg, int *i, int *j)
{
	while (!is_exp_delimiter(arg[*i]) && arg[*i])
	{
		(*i)++;
		(*j)++;
	}
}

void	init_exp(t_exp *exp, int status, t_data *parser, t_pipes *my_pipes)
{
	exp->expanded = 0;
	exp->no_element = 0;
	exp->status = status;
	exp->new_cmd = NULL;
	exp->new_line = NULL;
	exp->exp = NULL;
	exp->expansion = NULL;
	if (parser)
	{
		exp->parser = parser;
		exp->parsing = 1;
	}
	else
	{
		exp->parser = NULL;
		exp->parsing = 0;
	}
	if (my_pipes)
		exp->my_pipes = my_pipes;
	else
		exp->my_pipes = NULL;
}

void	handle_quotes_in_expansion(t_exp *expand, int *new_arg, int *arg)
{
	char	*temp;

	if (expand->no_element)
	{
		free (expand->new_cmd[*new_arg]);
		expand->new_cmd[*new_arg] = NULL;
		expand->no_element = 0;
	}
	else
	{
		if (!expand->expanded)
		{
			temp = handle_quotes(expand->new_cmd[*new_arg],
					expand->parser, expand);
			if (temp)
			{
				free (expand->new_cmd[*new_arg]);
				expand->new_cmd[*new_arg] = temp;
				temp = NULL;
			}
		}
		expand->expanded = 0;
		(*new_arg)++;
	}
	(*arg)++;
}

void	update_single_quote(char c, int *quote)
{
	if (c == '\'')
		*quote = !(*quote);
}
