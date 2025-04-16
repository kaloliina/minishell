#include "minishell.h"

void	append_char_heredoc(char **new_string, char *s, int i, t_pipes *my_pipes)
{
	char	additive[2];
	char	*temp;

	additive[0] = s[i];
	additive[1] = '\0';
	temp = *new_string;
	*new_string = ft_strjoin(*new_string, additive);
	if (!*new_string)
		handle_fatal_exit(MALLOC, my_pipes, my_pipes->command_node);
	free (temp);
}

void	append_char(char **new_string, char c, t_exp *expand)
{
	char	additive[2];
	char	*temp;

	additive[0] = c;
	additive[1] = '\0';
	temp = *new_string;
	*new_string = ft_strjoin(temp, additive);
	if (!*new_string)
	{
		free (temp);
		fatal_parsing_exit(expand->data, expand, NULL, MALLOC);
	}
	free (temp);
}

void	append_replacer(char **new_string, char *replacer, int is_freeable, t_exp *expand)
{
	char	*temp;

	temp = *new_string;
	*new_string = ft_strjoin(temp, replacer);
	if (!*new_string)
	{
		free (temp);
		free (replacer);
		fatal_parsing_exit(expand->data, expand, NULL, MALLOC);
	}
	free (temp);
	if (is_freeable)
		free (replacer);
}

char	*find_replacer(char *arg, int i, t_exp *expand, int new_arg)
{
	if (arg[i] == '?' && arg[i - 1] == '$')
		return (ft_itoa(expand->status));
	else
		return (find_envp(expand, 0, new_arg));
}

char	*find_exp(char *arg, int *i, int *k, t_data *data)
{
	int		j;
	char	*exp;

	j = 0;
	*k = *i;
	count_expandable(arg, i, &j);
	exp = ft_substr(arg, *k, j);
	if (!exp)
		fatal_parsing_exit(data, NULL, NULL, MALLOC);
	return (exp);
}

char	**find_envp_source(t_exp *expand)
{
	if (!expand->data)
		return (*expand->my_pipes->my_envp);
	else
		return (expand->data->envp);
}

//finds the correct line of envp to match the expandable in input
char	*find_envp(t_exp *expand, int i, int new_arg)
{
	int		len;
	int		envp_len;
	char	*replacer;
	char	**envp;

	len = ft_strlen(expand->exp);
	replacer = NULL;
	envp = find_envp_source(expand);
	while (envp[i])
	{
		envp_len = 0;
		while (envp[i][envp_len] && envp[i][envp_len] != '=')
			envp_len++;
		if (!ft_strncmp(expand->exp, envp[i], envp_len)
			&& !ft_strncmp(expand->exp, envp[i], len))
		{
			replacer = ft_substr(envp[i], len + 1,
					((ft_strlen(envp[i]) - len - 1)));
			if (!replacer)
			{
				if (expand->new_cmd)
					expand->new_cmd[new_arg + 1] = NULL;
				fatal_parsing_exit(expand->data, expand, NULL, MALLOC);
			}
		}
		i++;
	}
	return (replacer);
}
