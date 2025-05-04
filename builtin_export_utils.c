/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:09:50 by sojala            #+#    #+#             */
/*   Updated: 2025/05/04 18:09:51 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_existing_envp(char ***new_envp, char **cmd, char **envp, int i)
{
	int	j;
	int	k;

	j = 1;
	while (cmd[j])
	{
		k = 0;
		while (!is_exp_delimiter(cmd[j][k]))
			k++;
		if (!ft_strncmp(cmd[j], envp[i], k)
			&& envp[i][k] == '=')
			break ;
		j++;
	}
	if (cmd[j])
	{
		(*new_envp)[i] = ft_strdup(cmd[j]);
		return (j);
	}
	return (-1);
}

int	is_replacer_envp(char ***envp, char *arg)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	len++;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], arg, len))
			return (1);
		i++;
	}
	return (0);
}

int	export_validation(char **cmd, int i)
{
	int	j;

	while (cmd[i])
	{
		j = 0;
		if (!(ft_isalpha(cmd[i][j]) || cmd[i][j] == '_'))
		{
			print_error(ERR_EXPORT, cmd[i], NULL);
			return (-1);
		}
		while (cmd[i][j])
		{
			while (cmd[i][j] && !is_exp_delimiter(cmd[i][j]))
				j++;
			if (cmd[i][j] != '=' && cmd[i][j] != '\0')
			{
				print_error(ERR_EXPORT, cmd[i], NULL);
				return (-1);
			}
			j = ft_strlen(cmd[i]);
		}
		i++;
	}
	return (0);
}

int	is_valid_to_export(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '_' && arg[i + 1] == '=')
		return (0);
	if (!(ft_isalpha(arg[i]) || arg[i] == '_'))
		return (0);
	while (!is_exp_delimiter(arg[i]))
		i++;
	if (arg[i] != '=')
		return (0);
	return (1);
}

int	count_args_to_export(char **cmd)
{
	int	j;
	int	args;

	j = 1;
	args = 0;
	while (cmd[j])
	{
		if (is_valid_to_export(cmd[j]))
			args++;
		j++;
	}
	return (args);
}
