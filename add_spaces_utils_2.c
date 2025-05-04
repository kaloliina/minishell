/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_spaces_utils_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:08:59 by sojala            #+#    #+#             */
/*   Updated: 2025/05/04 18:09:04 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_triple_redirection(char *input, int i)
{
	if (input[i] == '<' && input[i + 1] && input[i + 2])
	{
		if (input[i + 1] == '<' && input[i + 2] == '<')
			return (1);
	}
	else if (input[i] == '>' && input[i + 1] && input[i + 2])
	{
		if (input[i + 1] == '>' && input[i + 2] == '>')
			return (1);
	}
	return (0);
}
