/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:09:55 by sojala            #+#    #+#             */
/*   Updated: 2025/05/06 17:59:09 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_fill_envp(char ***new_envp, char **cmd, char **envp,
	t_pipes *my_pipes)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp[i])
	{
		j = find_existing_envp(new_envp, cmd, envp, i);
		if (j < 0)
			(*new_envp)[i] = ft_strdup(envp[i]);
		if (!(*new_envp)[i])
			fatal_export_unset_error(*new_envp, my_pipes);
		i++;
	}
	return (add_exported_envp(new_envp, cmd, i, my_pipes));
}

int	add_exported_envp(char ***new_envp, char **cmd, int i,
	t_pipes *my_pipes)
{
	int	j;

	j = 1;
	while (cmd[j])
	{
		if (is_valid_to_export(cmd[j])
			&& !is_replacer_envp(my_pipes->my_envp, cmd[j]))
		{
			(*new_envp)[i] = ft_strdup(cmd[j]);
			if (!(*new_envp)[i])
				fatal_export_unset_error(*new_envp, my_pipes);
			i++;
		}
		j++;
	}
	return (i);
}

static void	sort_for_export(char ***export, char **envp,
	int elements, t_pipes *my_pipes)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (envp[i])
	{
		j = 0;
		k = 0;
		if (ft_strncmp(envp[i], "_=", 2))
		{
			while (envp[j])
			{
				if (i != j && (ft_strncmp(envp[j], "_=", 2))
					&& (ft_strcmp(envp[i], envp[j]) > 0))
					k++;
				j++;
			}
			(*export)[k] = add_quotes_export(envp, i);
			if (!(*export)[k])
				fatal_sort_for_export_error(*export, elements, my_pipes);
		}
		i++;
	}
}

static void	export_no_args(char **envp, t_pipes *my_pipes)
{
	int		elements;
	int		i;
	char	**export;

	elements = count_elements(envp) - 1;
	if (elements == -1)
		return ;
	export = ft_calloc((elements + 1), sizeof(char *));
	if (!export)
		fatal_exec_error(ERR_MALLOC, my_pipes, NULL, NULL);
	sort_for_export(&export, envp, elements, my_pipes);
	export[elements] = NULL;
	i = 0;
	while (export[i])
		ft_printf(1, "declare -x %s\n", export[i++]);
	free_array(export);
}

void	execute_export(char **cmd, char ***envp, t_pipes *my_pipes)
{
	int		i;
	int		args;
	char	**new_envp;

	if (!cmd[1])
		return (export_no_args(*envp, my_pipes));
	if (export_validation(cmd, 1) < 0)
		my_pipes->exit_status = 1;
	i = count_elements(*envp);
	args = count_args_to_export(cmd);
	if (!args)
		return ;
	new_envp = ft_calloc((i + 1 + args), sizeof(char *));
	if (!new_envp)
		fatal_exec_error(ERR_MALLOC, my_pipes, NULL, NULL);
	i = export_fill_envp(&new_envp, cmd, *envp, my_pipes);
	new_envp[i] = NULL;
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
}
