#include "minishell.h"

void	append_char(char **new_string, char *s, int i)
{
	char	additive[2];
	char	*temp;

	additive[0] = s[i];
	additive[1] = '\0';
	temp = *new_string;
	*new_string = ft_strjoin(*new_string, additive);
	free (temp);
}

void	append_replacer(char **new_string, char *replacer, int is_freeable)
{
	char	*temp;

	temp = *new_string;
	*new_string = ft_strjoin(*new_string, replacer);
	free (temp);
	if (is_freeable)
		free (replacer);
}

int	fill_replacer(char *new_line, char *line, int k, int j, int *l)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	if (k > 1)
	{
		while (i < (k - 2))
			new_line[(*l)++] = line[i++];
		if (line[i] != '"')
			new_line[(*l)++] = line[i++];
		else
			quote = 2;
	}
	i += (j + 1 + quote);
	return (i);
}

char	*find_replacer(char *arg, int i, t_exp *expand, char *exp)
{
	if (arg[i] == '?' && arg[i - 1] == '$')
		return (ft_itoa(expand->status));
	else
		return (find_envp(exp, expand->envp));
}

char	*find_exp(char *arg, int *i, int *k)
{
	int		j;
	char	*exp;

	j = 0;
	*k = *i;
	count_expandable(arg, i, &j);
	exp = ft_substr(arg, *k, j);
	//malloc protection
	return (exp);
}
