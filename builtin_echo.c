/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:09:31 by sojala            #+#    #+#             */
/*   Updated: 2025/05/04 18:09:32 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_echo(t_node *node)
{
	int		i;

	i = 1;
	if (!node->cmd[i])
	{
		ft_printf(1, "\n");
		return ;
	}
	if (node->cmd[i] && !ft_strcmp(node->cmd[i], "-n"))
		i++;
	while (node->cmd[i])
	{
		ft_printf(1, "%s", node->cmd[i]);
		if (node->cmd[i + 1])
			ft_printf(1, " ");
		i++;
	}
	if (ft_strcmp(node->cmd[1], "-n"))
		ft_printf(1, "\n");
}
