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

void	append_char(char **new_string, char *s, int i, t_data *data)
{
	char	additive[2];
	char	*temp;

	additive[0] = s[i];
	additive[1] = '\0';
	temp = *new_string;
	*new_string = ft_strjoin(*new_string, additive);
	if (!*new_string)
		fatal_parsing_exit(data, NULL, MALLOC);
	free (temp);
}

void	append_replacer(char **new_string, char *replacer, int is_freeable, t_data *data)
{
	char	*temp;

	temp = *new_string;
	*new_string = ft_strjoin(*new_string, replacer);
	if (!*new_string)
		fatal_parsing_exit(data, NULL, MALLOC);
	free (temp);
	if (is_freeable)
		free (replacer);
}

char	*find_replacer(char *arg, int i, t_exp *expand, char *exp)
{
	if (arg[i] == '?' && arg[i - 1] == '$')
		return (ft_itoa(expand->status));
	else
		return (find_envp(exp, expand));
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
		fatal_parsing_exit(data, NULL, MALLOC);
	return (exp);
}

//finds the correct line of envp to match the expandable in input
char	*find_envp(char *exp, t_exp *expand)
{
	int		i;
	int		len;
	int		envp_len;
	char	*replacer;

	i = 0;
	len = ft_strlen(exp);
	replacer = NULL;
	while (expand->data->envp[i])
	{
		envp_len = 0;
		while (expand->data->envp[i][envp_len]
			&& expand->data->envp[i][envp_len] != '=')
			envp_len++;
		if (!ft_strncmp(exp, expand->data->envp[i], envp_len)
			&& !ft_strncmp(exp, expand->data->envp[i], len))
		{
			replacer = ft_substr(expand->data->envp[i], len + 1,
					((ft_strlen(expand->data->envp[i]) - len - 1)));
			if (!replacer)
				fatal_parsing_exit(expand->data, NULL, MALLOC);
		}
		i++;
	}
	return (replacer);
}
