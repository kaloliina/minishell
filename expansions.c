#include "minishell.h"

char	*find_envp(char *exp, char **envp)
{
	int		i;
	int		len;
	char	*replacer;

	i = 0;
	len = ft_strlen(exp);
	replacer = NULL;
	while (envp[i])
	{
		if (!ft_strncmp(exp, envp[i], len))
			replacer = ft_substr(envp[i], len + 1, ((ft_strlen(envp[i]) - len - 1)));
		i++;
	}
	free (exp);
	return (replacer);
}

char	*add_replacer(char *line, char *replacer, int k, int j)
{
	int		i;
	int		l;
	int		m;
	int		len;
	int		quote;
	int		replacer_len;
	char	*new_line;

	i = 0;
	l = 0;
	m = 0;
	quote = 0;
	replacer_len = ft_strlen(replacer);
	len = (ft_strlen(line) + replacer_len) - j;
	new_line = malloc(len + 1);
	if (!new_line)
	{
		ft_printf(2, "minishell: memory allocation failure\n");
		exit (1);
	}
	if (k > 1)
	{
		while (i < (k - 2))
			new_line[l++] = line[i++];
		if (line[i] != '"')
			new_line[l++] = line[i++];
		else
			quote = 1;
	}
	while (replacer_len > 0)
	{
		new_line[l++] = replacer[m++];
		replacer_len--;
	}
	i += (j + 1 + quote);
	while (line[i])
		new_line[l++] = line[i++];
	new_line[l] = '\0';
	return (new_line);
}

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
	while (cmd[i])
	{
		if (i == arg)
			i++;
		else
			new_cmd[j++] = ft_strdup(cmd[i++]);
	}
	new_cmd[j] = NULL;
	free_array(cmd);
	return (new_cmd);
}

int	is_exp_delimiter(char c)
{
	return (!(ft_isalnum(c) || c == '_'));
}
/*THIS LEAKS!! NEW_LINE IS SOMEHOW NOT FREED. HOW??*/
char	**handle_expansion_cmds(char **cmd, char **envp)
{
	int		i;
	int		j;
	int		k;
	int		arg;
	int		quote;
	char	*exp;
	char	*replacer;
	char	*new_line;

	i = 0;
	arg = 0;
	k = 0;
	quote = 0;
	while (cmd[arg])
	{
		i = 0;
		while (cmd[arg][i])
		{
			if (cmd[arg][i] == '$' && cmd[arg][i + 1] && !quote)
			{
				j = 0;
				i++;
				k = i;
				while (!is_exp_delimiter(cmd[arg][i]))
				{
					i++;
					j++;
				}
				exp = ft_substr(cmd[arg], k, j);
				if (exp && *exp)
				{
					replacer = find_envp(exp, envp);
					if (replacer)
					{
						new_line = add_replacer(cmd[arg], replacer, k, j);
						free (replacer);
						cmd[arg] = new_line;
						//new_line = NULL;
					}
					else if (k > 1)
					{
						new_line = ft_substr(cmd[arg], 0, k - 1);
						if (cmd[arg][i + 1])
						{
							char	*end = ft_substr(cmd[arg], i, ft_strlen(cmd[arg]) - i);
							char	*temp = ft_strjoin(new_line, end);
							cmd[arg] = temp;
						}
						else
							cmd[arg] = new_line;
						//new_line = NULL;
					}
					else
					{
						cmd = delete_element(cmd, arg);
						break ;
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
		arg++;
	}
	return (cmd);
}

char	*handle_expansion_cmd(char *line, char **envp)
{
	int		i;
	int		j;
	int		k;
	int		quote;
	char	*exp;
	char	*replacer;
	char	*new_line;

	i = 0;
	k = 0;
	quote = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && !quote)
		{
			j = 0;
			i++;
			k = i;
			while (!is_exp_delimiter(line[i]))
			{
				i++;
				j++;
			}
				exp = ft_substr(line, k, j);
				if (exp && *exp)
				{
					replacer = find_envp(exp, envp);
					if (replacer)
					{
						new_line = add_replacer(line, replacer, k, j);
						free (replacer);
					}
					else
					{
						new_line = ft_strdup("");
					}
					free (line);
					line = NULL;
					line = new_line;
					new_line = NULL;
				}
			i = k;
		}
		else
		{
			if (line[i] == 39)
				quote = !quote;
			i++;
		}
	}
	return (line);
}

char	*handle_expansion_filename(char *file, char **envp)
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
			if (exp && *exp)
			{
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
