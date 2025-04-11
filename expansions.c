#include "minishell.h"

//finds the correct line of envp to match the expandable in input
char	*find_envp(char *exp, char **envp)
{
	int		i;
	int		len;
	int		envp_len;
	char	*replacer;

	i = 0;
	len = ft_strlen(exp);
	replacer = NULL;
	while (envp[i])
	{
		envp_len = 0;
		while (envp[i][envp_len] && envp[i][envp_len] != '=')
			envp_len++;
		if (!ft_strncmp(exp, envp[i], envp_len)
			&& !ft_strncmp(exp, envp[i], len))
			replacer = ft_substr(envp[i], len + 1, ((ft_strlen(envp[i]) - len - 1)));
			//malloc protection
		i++;
	}
	return (replacer);
}

//makes a new cmd array without the invalid element
char	**delete_element(char **cmd, int arg)
{
	int		i;
	int		j;
	int		elements;
	char	**new_cmd;

	i = 0;
	j = 0;
	elements = count_elements(cmd);
	new_cmd = malloc(sizeof(char *) * elements);
	//malloc protection
	while (cmd[i])
	{
		if (i == arg)
		{
			new_cmd[j++] = ft_strdup(cmd[i]);
			if (!new_cmd[j - 1])
			{
				ft_printf(2, MALLOC);
				free_array(new_cmd); //must also free everything else
				exit (1);
			}
		}
		i++;
	}
	new_cmd[j] = NULL;
	return (new_cmd);
}

char	**handle_expansion_cmds(char **cmd, char **envp, int status)
{
	int		arg;
	int		new_arg;
	t_exp	expand;

	arg = 0;
	new_arg = 0;
	init_exp(&expand, status, envp);
	expand.new_cmd = malloc(sizeof(char *) * (count_elements(cmd) + 1));
	//malloc protection
	while (cmd[arg])
	{
		if (!ft_strchr(cmd[arg], '$'))
			expand.new_cmd[new_arg++] = handle_quotes(cmd[arg++]);
		else
			expand_cmd(cmd, &expand, &arg, &new_arg);
	}
	expand.new_cmd[new_arg] = NULL;
	return (expand.new_cmd);
}

//goes through filename string and expands it
char	*handle_expansion_filename(char *file, char **envp, int status)
{
	char	*new_file;
	t_exp	expand;

	init_exp(&expand, status, envp);
	if (!ft_strchr(file, '$'))
		new_file = handle_quotes(file);
	else
		new_file = expand_line(file, &expand);
	return (new_file);
}
