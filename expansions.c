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

char	**handle_cmd_expansion(char **cmd, char **envp, int status)
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
char	*handle_filename_expansion(char *file, char **envp, int status)
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

char	*handle_quotes(char *s)
{
	int		i;
	int		j;
	int		s_quote;
	int		d_quote;
	char	*new;

	i = 0;
	j = 0;
	s_quote = 0;
	d_quote = 0;
	new = malloc(ft_strlen(s) + 1);
	//malloc protection
	while (s[i])
	{
		if ((s[i] == 34 && d_quote) || (s[i] == 39 && s_quote))
			new[j++] = s[i];
		else if (s[i] == 34 && !d_quote)
			s_quote = !s_quote;
		else if (s[i] == 39 && !s_quote)
			d_quote = !d_quote;
		else
			new[j++] = s[i];
		i++;
	}
	new[j] = '\0';
	return (new);
}

void	handle_cmd(t_node *tmp, char **envp, int status)
{
	char	**new_cmd;

	new_cmd = handle_cmd_expansion(tmp->cmd, envp, status);
	if (new_cmd)
	{
		free_array(tmp->cmd);
		tmp->cmd = new_cmd;
		new_cmd = NULL;
	}
}

void	handle_filename(t_node *tmp, char **envp, int status)
{
	char	*new_file;

	new_file = handle_filename_expansion(tmp->file, envp, status);
	if (new_file)
	{
		free (tmp->file);
		tmp->file = new_file;
		new_file = NULL;
	}
}