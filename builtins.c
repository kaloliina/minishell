#include "minishell.h"

void	execute_echo(t_node *node, char ***envp)
{
	int		i;
	char	*temp;

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

void	execute_env(char ***envp)
{
	int	i;

	i = 0;
	while ((*envp)[i])
	{
		ft_printf(1, "%s\n", (*envp)[i]);
		i++;
	}
}

void	execute_pwd(void)
{
	char	*buf;

	buf = malloc(4096);
	if (!buf)
	{
		ft_printf(2, "%s\n", MALLOC); //must free my_pipes and nodes
		exit (1);
	}
	getcwd(buf, 4096);
	if (!buf)
		perror("minishell");
	else
		ft_printf(1, "%s\n", buf);
	free (buf);
}

void	export_no_args(char **envp)
{
	int	elements;
	int	i;
	int	j;
	int	k;
	char	**export;

	elements = count_elements(envp);
	export = malloc(sizeof(char *) * (elements + 1));
	//malloc protection
	export = sort_for_export(export, envp, elements);
	export[elements] = NULL;
	i = 0;
	while (export[i])
		ft_printf(1, "declare -x %s\n", export[i++]);
	free_array(export);
}

void	execute_export(char **cmd, char ***envp)
{
	int		i;
	int		j;
	int		args;
	char	**new_envp;

	if (!cmd[1])
		return (export_no_args(*envp));
	i = count_elements(*envp);
	args = 1;
	while (cmd[args + 1])
		args++;
	new_envp = malloc(sizeof(char *) * (i + 1 + args));
	//malloc protection
	i = add_existing_envp(new_envp, *envp);
	i = add_exported_envp(new_envp, cmd, i);
	new_envp[i] = NULL;
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
}

void	execute_cd(char **cmd)
{
	if (cmd[2])
	{
		ft_printf(2, "minishell: cd: too many arguments\n");
		//free everything
		exit (1);
	}
	if (chdir(cmd[1]) == -1)
		perror("minishell");
}

void	execute_unset(char **cmd, char ***envp)
{
	int		i;
	int		args;
	char	**new_envp;

	i = count_elements(*envp);
	args = 1;
	while (cmd[args + 1])
		args++;
	new_envp = malloc(sizeof(char *) * ((i - args) + 2));
	//malloc protection
	new_envp = fill_unset_envp(new_envp, cmd, *envp);
	free_array(*envp);
	*envp = NULL;
	*envp = new_envp;
}

void	execute_exit(char **cmd, t_pipes *my_pipes)
{
	int	status;
	int	i;
	int	is_num;

	status = 0;
	is_num = 1;
	i = 0;
	if (cmd[1] != NULL)
	{
		while (cmd[1][i] != '\0')
		{
			if (!(cmd[1][i] >= 48 && cmd[1][i] <= 57))
				is_num = 0;
			i++;
		}
		if (is_num == 0)
		{
			ft_printf(2, "%s: %s: %s\n", cmd[0], cmd[1], ERR_NUM);
			status = 2;
		}
		else
			status = ft_atoi(cmd[1]);
	}
	printf("exit status: %d\n", status);
	free_array(*my_pipes->my_envp);
	free_my_pipes(my_pipes);
	exit (status);
}
