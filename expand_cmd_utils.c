/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:29:09 by sojala            #+#    #+#             */
/*   Updated: 2025/05/05 18:30:09 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_new_cmd(char **cmd, t_data *parser, t_exp *expand)
{
	expand->new_cmd = ft_calloc((count_elements(cmd) + 1), sizeof(char *));
	if (!expand->new_cmd)
		fatal_parsing_error(parser, expand, NULL, ERR_MALLOC);
}

void	init_new_arg(t_exp *expand, int *new_arg)
{
	expand->new_cmd[*new_arg] = ft_strdup("");
	if (!expand->new_cmd[*new_arg])
		fatal_parsing_error(expand->parser, expand, NULL, ERR_MALLOC);
}
