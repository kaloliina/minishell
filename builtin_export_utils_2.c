/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:08:15 by sojala            #+#    #+#             */
/*   Updated: 2025/05/05 14:08:17 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_quotes_export_helper(char **envp, int i, int j, char *temp2)
{
	char	*res;
	char	*temp3;

	res = ft_substr(envp[i], j, (ft_strlen(envp[i]) - j));
	if (!res)
	{
		free (temp2);
		return (NULL);
	}
	temp3 = ft_strjoin(temp2, res);
	free (temp2);
	temp2 = NULL;
	free (res);
	res = NULL;
	if (!temp3)
		return (NULL);
	res = ft_strjoin(temp3, "\"");
	free (temp3);
	temp3 = NULL;
	return (res);
}

char	*add_quotes_export(char **envp, int i)
{
	int		j;
	char	*temp1;
	char	*temp2;
	char	*res;

	j = 0;
	while (envp[i][j] && envp[i][j] != '=')
		j++;
	if (envp[i][j] == '\0')
		return (ft_strdup(envp[i]));
	temp1 = ft_substr(envp[i], 0, j + 1);
	if (!temp1)
		return (NULL);
	temp2 = ft_strjoin(temp1, "\"");
	free (temp1);
	temp1 = NULL;
	if (!temp2)
		return (NULL);
	j++;
	res = add_quotes_export_helper(envp, i, j, temp2);
	return (res);
}
