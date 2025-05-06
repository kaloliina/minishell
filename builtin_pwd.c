/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:10:00 by sojala            #+#    #+#             */
/*   Updated: 2025/05/06 10:35:47 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_pwd(t_pipes *my_pipes, char ***envp, int i, t_exp *expand)
{
	char	*buf;

	buf = malloc(4096);
	if (!buf)
		fatal_exec_error(ERR_MALLOC, my_pipes, NULL, NULL);
	if (!getcwd(buf, 4096))
		perror("minishell: getcwd");
	else if (!envp && !i)
		ft_printf(1, "%s\n", buf);
	else
	{
		free ((*envp)[i]);
		(*envp)[i] = ft_strjoin("PWD=", buf);
		if (!(*envp)[i])
		{
			free (buf);
			buf = NULL;
			fatal_pwd_error(ERR_MALLOC, my_pipes, i, expand);
		}
	}
	free (buf);
	buf = NULL;
}
