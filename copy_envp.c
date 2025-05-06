/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:10:25 by sojala            #+#    #+#             */
/*   Updated: 2025/05/06 10:17:01 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	convert_new_shlvl(int i, int level, char ***my_envp)
{
	char	*level_nbr;

	level_nbr = ft_itoa(level);
	if (!level_nbr)
	{
		print_error(ERR_MALLOC, NULL, NULL);
		free_array(*my_envp);
		exit (1);
	}
	(*my_envp)[i] = ft_strjoin("SHLVL=", level_nbr);
	free (level_nbr);
	level_nbr = NULL;
	if (!(*my_envp)[i])
	{
		print_error(ERR_MALLOC, NULL, NULL);
		free_array(*my_envp);
		exit (1);
	}
}

static int	handle_shlvl(char **envp, int i, char ***my_envp)
{
	char	*level_nbr;
	int		level;

	if (!ft_strncmp(envp[i], "SHLVL=", 6))
	{
		level_nbr = ft_substr(envp[i], 6, ft_strlen(envp[i]) - 6);
		if (!level_nbr)
		{
			print_error(ERR_MALLOC, NULL, NULL);
			free_array(*my_envp);
			exit (1);
		}
		level = ft_atoi(level_nbr);
		level++;
		free (level_nbr);
		level_nbr = NULL;
		convert_new_shlvl(i, level, my_envp);
		i++;
	}
	return (i);
}

char	**copy_envp(char **envp)
{
	char	**my_envp;
	int		i;

	i = count_elements(envp);
	my_envp = ft_calloc((i + 1), sizeof(char *));
	if (!my_envp)
	{
		print_error(ERR_MALLOC, NULL, NULL);
		exit (1);
	}
	i = 0;
	while (envp[i])
	{
		i = handle_shlvl(envp, i, &my_envp);
		my_envp[i] = ft_strdup(envp[i]);
		if (!my_envp[i])
		{
			print_error(ERR_MALLOC, NULL, NULL);
			free_array(my_envp);
			exit (1);
		}
		i++;
	}
	my_envp[i] = NULL;
	return (my_envp);
}
