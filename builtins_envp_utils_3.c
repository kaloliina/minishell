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
				handle_fatal_exit(MALLOC, my_pipes, NULL, NULL);
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
