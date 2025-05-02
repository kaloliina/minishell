#include "minishell.h"

static void	find_and_update_oldpwd(char ***envp, char *old_pwd,
	t_pipes *my_pipes)
{
	int	i;

	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], "OLDPWD=", 7))
		{
			free ((*envp)[i]);
			(*envp)[i] = ft_strjoin("OLD", old_pwd);
			free (old_pwd);
			if (!(*envp)[i])
				fatal_pwd_error(MALLOC, my_pipes, i);
			return ;
		}
		i++;
	}
}

void	update_envp(t_pipes *my_pipes)
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
				fatal_exec_error(MALLOC, my_pipes, NULL, NULL);
			old_pwd_i = i;
			break ;
		}
		i++;
	}
	find_and_update_oldpwd(envp, old_pwd, my_pipes);
	free ((*envp)[old_pwd_i]);
	(*envp)[old_pwd_i] = NULL;
	execute_pwd(my_pipes, envp, old_pwd_i);
}

static void	cd_no_args(t_exp *expand, t_pipes *my_pipes)
{
	char	*expansion;

	expand->exp = ft_strdup("HOME");
	if (!expand->exp)
		fatal_exec_error(MALLOC, my_pipes, NULL, NULL);
	expansion = find_envp(expand, 0);
	if (!expansion)
	{
		ft_printf(2, "minishell: cd: HOME not set\n");
		my_pipes->exit_status = 1;
	}
	else if (chdir(expansion) == -1)
		perror("minishell: cd");
	else
		update_envp(my_pipes);
	free (expansion);
	free (expand->exp);
}

void	execute_cd(char **cmd, t_pipes *my_pipes)
{
	t_exp	expand;

	init_exp(&expand, 0, NULL, my_pipes);
	if (count_elements(cmd) == 1)
		cd_no_args(&expand, my_pipes);
	else if (count_elements(cmd) > 2)
	{
		ft_printf(2, "minishell: %s: %s", cmd[0], ERR_ARG);
		my_pipes->exit_status = 1;
	}
	else if (count_elements(cmd) == 2)
	{
		if (chdir(cmd[1]) == -1)
		{
			ft_printf(2, "minishell: %s: %s: ", cmd[0], cmd[1]);
			perror("");
			my_pipes->exit_status = 1;
		}
		else
			update_envp(my_pipes);
	}
}
