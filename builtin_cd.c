#include "minishell.h"

static void	find_and_update_oldpwd(char ***envp, char *old_pwd,
	t_pipes *my_pipes, t_exp *expand)
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
				fatal_pwd_error(ERR_MALLOC, my_pipes, i, expand);
			return ;
		}
		i++;
	}
}

void	update_envp(t_pipes *my_pipes, t_exp *expand)
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
	find_and_update_oldpwd(envp, old_pwd, my_pipes, expand);
	free ((*envp)[old_pwd_i]);
	(*envp)[old_pwd_i] = NULL;
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
	else if (chdir(expand->expansion) == -1)
		perror("minishell: cd");
	else
		update_envp(my_pipes, expand);
	free (expand->expansion);
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
		print_error("cd: %s", ERR_ARG, NULL);
		my_pipes->exit_status = 1;
	}
	else if (count_elements(cmd) == 2)
	{
		if (chdir(cmd[1]) == -1)
		{
			print_error("cd: %s: ", cmd[1], NULL);
			perror("");
			my_pipes->exit_status = 1;
		}
		else
			update_envp(my_pipes, &expand);
	}
}
