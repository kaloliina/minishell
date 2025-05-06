/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:09:26 by sojala            #+#    #+#             */
/*   Updated: 2025/05/06 10:34:58 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_oldpwd(char ***envp, char *old_pwd, int i)
{
	int	unset;

	unset = 0;
	free ((*envp)[i]);
	if (old_pwd)
	{
		(*envp)[i] = ft_strjoin("OLD", old_pwd);
		free (old_pwd);
		old_pwd = NULL;
	}
	else
	{
		(*envp)[i] = ft_strdup("OLDPWD");
		unset = -1;
	}
	return (unset);
}

static int	find_and_update_oldpwd(char ***envp, char *old_pwd,
	t_pipes *my_pipes, t_exp *expand)
{
	int	i;
	int	set;

	i = 0;
	set = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], "OLDPWD=", 7))
		{
			set = update_oldpwd(envp, old_pwd, i);
			if (!(*envp)[i])
				fatal_pwd_error(ERR_MALLOC, my_pipes, i, expand);
			break ;
		}
		i++;
	}
	if (!(*envp)[i])
	{
		set = -1;
		free (old_pwd);
		old_pwd = NULL;
	}
	return (set);
}

static void	update_envp(t_pipes *my_pipes, t_exp *expand)
{
	int		i;
	int		old_pwd_i;
	char	*old_pwd;
	char	***envp;

	i = 0;
	old_pwd_i = 0;
	old_pwd = NULL;
	envp = my_pipes->my_envp;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], "PWD=", 4))
		{
			old_pwd = ft_strdup((*envp)[i]);
			if (!old_pwd)
				fatal_pwd_error(ERR_MALLOC, my_pipes, 0, expand);
			old_pwd_i = i;
			break ;
		}
		i++;
	}
	if (find_and_update_oldpwd(envp, old_pwd, my_pipes, expand) != -1)
		execute_pwd(my_pipes, envp, old_pwd_i, expand);
}

static void	cd_no_args(t_exp *expand, t_pipes *my_pipes)
{
	expand->exp = ft_strdup("HOME");
	if (!expand->exp)
		fatal_exec_error(ERR_MALLOC, my_pipes, NULL, NULL);
	expand->expansion = find_envp(expand, 0);
	if (!expand->expansion)
	{
		print_error("cd: HOME not set\n", NULL, NULL);
		my_pipes->exit_status = 1;
	}
	else if (chdir(expand->expansion) < 0)
	{
		perror("minishell: cd");
		my_pipes->exit_status = 1;
	}
	else
		update_envp(my_pipes, expand);
	free (expand->expansion);
	expand->expansion = NULL;
	free (expand->exp);
	expand->exp = NULL;
}

void	execute_cd(char **cmd, t_pipes *my_pipes)
{
	t_exp	expand;

	init_exp(&expand, 0, NULL, my_pipes);
	if (count_elements(cmd) == 1)
		cd_no_args(&expand, my_pipes);
	else if (count_elements(cmd) > 2)
	{
		print_error("cd: %s", ERR_ARG, NULL);
		my_pipes->exit_status = 1;
	}
	else if (count_elements(cmd) == 2)
	{
		if (chdir(cmd[1]) < 0)
		{
			print_error("cd: %s: ", cmd[1], NULL);
			perror("");
			my_pipes->exit_status = 1;
		}
		else
			update_envp(my_pipes, &expand);
	}
}
