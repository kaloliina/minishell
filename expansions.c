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
/*
char	**handle_expansion_cmds(char **cmd, char **envp, int status)
{
	int		i;
	int		j;
	int		k;
	int		arg;
	int		new_arg;
	int		quote;
	int		no_elem;
	int		expanded;
	char	*exp;
	char	*replacer;
	char	*temp;
	char	**new_cmd;

	i = 0;
	arg = 0;
	k = 0;
	quote = 0;
	no_elem = 0;
	expanded = 0;
	new_cmd = malloc(sizeof(char *) * (count_elements(cmd) + 1));
	//malloc protection
	new_arg = 0;
	while (cmd[arg])
	{
		if (!ft_strchr(cmd[arg], '$'))
			new_cmd[new_arg++] = handle_quotes_helper(cmd[arg++]);
		else
		{
			i = 0;
			new_cmd[new_arg] = ft_strdup("");
			//malloc protection
			while (cmd[arg][i])
			{
				if (cmd[arg][i] == '$' && cmd[arg][i + 1] && !quote)
				{
					j = 0;
					i++;
					k = i;
					while (!is_exp_delimiter(cmd[arg][i]) && cmd[arg][i])
					{
						i++;
						j++;
					}
					exp = ft_substr(cmd[arg], k, j);
					//malloc protection
					if ((exp && *exp) || (cmd[arg][i] == '?' && cmd[arg][i - 1] == '$'))
					{
						if (cmd[arg][i] == '?' && cmd[arg][i - 1] == '$')
						{
							j = 1;
							replacer = ft_itoa(status);
						}
						else
							replacer = find_envp(exp, envp);
						if (replacer)
						{
							if (!is_quote(cmd[arg]))
								expanded = 1;
							append_replacer(&(new_cmd[new_arg]), replacer);
							i = k + ft_strlen(exp);
							free (exp);
						}
						else
						{
							if (cmd[arg][i] && cmd[arg][i + 1])
							{
								i = k;
								printf("now we here %c\n", cmd[arg][i]);
							}
							else
							{
								no_elem = 1;
								break ;
							}
						}
					}
				}
				else
				{
					if (cmd[arg][i] == 39)
						quote = !quote;
					append_char(&(new_cmd[new_arg]), cmd[arg], i);
					i++;
				}
			}
			if (no_elem)
				no_elem = 0;
			else
			{
				if (!expanded)
				{
					temp = handle_quotes_helper(new_cmd[new_arg]);
					if (temp)
					{
						new_cmd[new_arg] = temp;
						temp = NULL;
					}
				}
				new_arg++;
			}
			arg++;
		}
	}
	new_cmd[new_arg] = NULL;
	return (new_cmd);
}*/

//goes through cmd array and handles expansions, making a completely new cmd array
char	**handle_expansion_cmds(char **cmd, char **envp, int status)
{
	int		i;
	int		j;
	int		k;
	int		arg;
	int		new_arg;
	int		quote;
	int		no_elem;
	int		expanded;
	char	*exp;
	char	*replacer;
	char	*new_line;
	char	*end;
	char	*temp;
	char	**new_cmd;
	char	**tmp;

	i = 0;
	arg = 0;
	k = 0;
	quote = 0;
	no_elem = 0;
	expanded = 0;
	new_cmd = malloc(sizeof(char *) * (count_elements(cmd) + 1));
	//malloc protection
	new_arg = 0;
	while (cmd[arg])
	{
		if (!ft_strchr(cmd[arg], '$'))
			new_cmd[new_arg++] = handle_quotes_helper(cmd[arg++]);
		else
		{
			i = 0;
			while (cmd[arg][i])
			{
				if (cmd[arg][i] == '$' && cmd[arg][i + 1] && !quote)
				{
					j = 0;
					i++;
					k = i;
					while (!is_exp_delimiter(cmd[arg][i]) && cmd[arg][i])
					{
						i++;
						j++;
					}
					exp = ft_substr(cmd[arg], k, j);
					//malloc protection
					if ((exp && *exp) || (cmd[arg][i] == '?' && cmd[arg][i - 1] == '$'))
					{
						if (cmd[arg][i] == '?' && cmd[arg][i - 1] == '$')
						{
							j = 1;
							replacer = ft_itoa(status);
						}
						else
							replacer = find_envp(exp, envp);
						if (replacer)
						{
							if (!is_quote(cmd[arg]))
								expanded = 1;
							new_line = add_replacer(cmd[arg], replacer, k, j);
							free (replacer);
							free (cmd[arg]);
							cmd[arg] = new_line;
							new_line = NULL;
						}
						else if (k > 1)
						{
							new_line = ft_substr(cmd[arg], 0, k - 1);
							if (cmd[arg][i] && cmd[arg][i + 1])
							{
								end = ft_substr(cmd[arg], i, (ft_strlen(cmd[arg]) - i));
								//malloc protection
								temp = ft_strjoin(new_line, end);
								//malloc protection
								free (cmd[arg]);
								cmd[arg] = temp;
								free (new_line);
								free (end);
								end = NULL;
								new_line = NULL;
								temp = NULL;
							}
							else
							{
								free (cmd[arg]);
								cmd[arg] = new_line;
								new_line = NULL;
							}
						}
						else
						{
							if (cmd[arg][i] && cmd[arg][i + 1])
							{
								new_line = ft_substr(cmd[arg], i, (ft_strlen(cmd[arg]) - i));
								//malloc protection
								free (cmd[arg]);
								cmd[arg] = new_line;
								new_line = NULL;
							}
							else
							{
								no_elem = 1;
								break ;
							}
						}
					}
					i = k;
				}
				else
				{
					if (cmd[arg][i] == 39)
						quote = !quote;
					i++;
				}
			}
			if (no_elem)
				no_elem = 0;
			else
			{
				if (!expanded)
				{
					new_line = handle_quotes_helper(cmd[arg]);
					if (new_line)
					{
						new_cmd[new_arg] = new_line;
						new_line = NULL;
					}
				}
				new_arg++;
			}
			arg++;
		}
	}
	new_cmd[new_arg] = NULL;
	return (new_cmd);
}

//goes through filename string and expands it
char	*handle_expansion_filename(char *file, char **envp, int status)
{
	int		i;
	int		j;
	int		k;
	int		quote;
	char	*exp;
	char	*replacer;
	char	*new_file;

	i = 0;
	k = 0;
	quote = 0;
	if (!ft_strchr(file, '$'))
		return (NULL);
	while (file[i])
	{
		if (file[i] == '$' && file[i + 1] && !quote)
		{
			j = 0;
			i++;
			k = i;
			while (!is_exp_delimiter(file[i]))
			{
				i++;
				j++;
			}
			exp = ft_substr(file, k, j);
			//malloc protection
			if ((exp && *exp) || (file[i] == '?' && file[i - 1] == '$'))
			{
				if (file[i] == '?' && file[i - 1] == '$')
				{
					j = 1;
					replacer = ft_itoa(status);
				}
				else
					replacer = find_envp(exp, envp);
				if (replacer)
				{
					new_file = add_replacer(file, replacer, k, j);
					free (replacer);
					free (file);
					file = NULL;
					file = new_file;
					new_file = NULL;
				}
			}
			i = k;
		}
		else
		{
			if (file[i] == 39)
				quote = !quote;
			i++;
		}
	}
	return (file);
}
