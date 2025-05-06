/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:44:19 by sojala            #+#    #+#             */
/*   Updated: 2025/05/06 10:48:21 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_expand(t_exp *expand)
{
	if (expand)
	{
		if (expand->exp)
		{
			free (expand->exp);
			expand->exp = NULL;
		}
		if (expand->expansion)
		{
			free (expand->expansion);
			expand->expansion = NULL;
		}
		if (expand->new_cmd)
			free_array(expand->new_cmd);
		if (expand->new_line)
		{
			free (expand->new_line);
			expand->new_line = NULL;
		}
	}
}
