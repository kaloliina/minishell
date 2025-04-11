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

void	append_replacer(char **new_string, char *replacer)
{
	char	*temp;

	temp = *new_string;
	*new_string = ft_strjoin(*new_string, replacer);
	free (temp);
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

//makes a new string that is expanded
char	*add_replacer(char *line, char *replacer, int k, int j)
{
	int		i;
	int		l;
	int		m;
	int		len;
	int		replacer_len;
	char	*new_line;

	i = 0;
	l = 0;
	m = 0;
	replacer_len = ft_strlen(replacer);
	len = (ft_strlen(line) + replacer_len) - j;
	new_line = malloc(len + 1);
	//malloc protection
	i = fill_replacer(new_line, line, k, j, &l);
	while (replacer_len > 0)
	{
		new_line[l++] = replacer[m++];
		replacer_len--;
	}
	while (line[i])
		new_line[l++] = line[i++];
	new_line[l] = '\0';
	return (new_line);
}

char	*find_replacer(char *arg, int i, t_exp *expand, char *exp)
{
	if (arg[i] == '?' && arg[i - 1] == '$')
		return (ft_itoa(expand->status));
	else
		return (find_envp(exp, expand->envp));
}

void	count_expandable(char *arg, int *i, int *j)
{
	while (!is_exp_delimiter(arg[*i]) && arg[*i])
	{
		(*i)++;
		(*j)++;
	}
}

void	init_exp(t_exp *exp, int status, char **envp)
{
	exp->expanded = 0;
	exp->no_element = 0;
	exp->status = status;
	exp->envp = envp;
	exp->new_cmd = NULL;
}

void	handle_quotes_in_expansion(t_exp *expand, int *new_arg, int *arg)
{
	char	*temp;

	if (expand->no_element)
		expand->no_element = 0;
	else
	{
		if (!expand->expanded)
		{
			temp = handle_quotes(expand->new_cmd[*new_arg]);
			if (temp)
			{
				expand->new_cmd[*new_arg] = temp;
				temp = NULL;
			}
		}
		(*new_arg)++;
	}
	(*arg)++;
}

int	expand_cmd_helper(char *arg, t_exp *expand, int new_arg, int i)
{
	int		k;
	int		j;
	char	*exp;
	char	*replacer;

	j = 0;
	k = i;
	count_expandable(arg, &i, &j);
	exp = ft_substr(arg, k, j);
	//malloc protection
	if ((exp && *exp) || (arg[i] == '?' && arg[i - 1] == '$'))
	{
		replacer = find_replacer(arg, i, expand, exp);
		if (replacer)
		{
			if (!is_quote(arg))
				expand->expanded = 1;
			append_replacer(&(expand->new_cmd[new_arg]), replacer);
			if (exp && *exp)
				i = k + ft_strlen(exp);
			else
				i = k + 1;
			free (replacer);
		}
		else if (arg[i] && arg[i + 1] && arg[i] != '"')
			i = k + ft_strlen(exp);
		else if (k == 1)
			expand->no_element = 1;
		free (exp);
	}
	return (i);
}

void	expand_cmd(char **cmd, t_exp *expand, int *arg, int *new_arg)
{
	int		i;
	int		quote;
	// char	*temp;

	i = 0;
	quote = 0;
	expand->new_cmd[*new_arg] = ft_strdup("");
	//malloc protection
	while (cmd[*arg][i])
	{
		if (cmd[*arg][i] == '$' && cmd[*arg][i + 1] && !quote)
			i = expand_cmd_helper(cmd[*arg], expand, *new_arg, i + 1);
		else
		{
			if (cmd[*arg][i] == 39)
				quote = !quote;
			append_char(&expand->new_cmd[*new_arg], cmd[*arg], i);
			i++;
		}
	}
	handle_quotes_in_expansion(expand, new_arg, arg);
}
//do we need to handle else if (file[i] && file[i + 1] && file[i] != '"") here as well as in cmd? but then it doesn't work in heredoc??
int	expand_line_helper(char *file, char **new_file, t_exp *expand, int i)
{
	int		k;
	int		j;
	char	*exp;
	char	*replacer;

	i++;
	k = i;
	j = 0;
	count_expandable(file, &i, &j);
	exp = ft_substr(file, k, j);
	//malloc protection
	if ((exp && *exp) || (file[i] == '?' && file[i - 1] == '$'))
	{
		replacer = find_replacer(file, i, expand, exp);
		if (replacer)
		{
			if (!is_quote(file))
				expand->expanded = 1;
			append_replacer(new_file, replacer);
			if (exp && *exp)
				i = k + ft_strlen(exp);
			else
				i = k + 1;
			free (replacer);
		}
		else if (file[i] && file[i + 1])
			i = k + ft_strlen(exp);
		else
		{
			if (k == 1)
				append_replacer(new_file, file);
			i = ft_strlen(file);
		}
		free (exp);
	}
	return (i);
}

char	*expand_line(char *file, t_exp *expand)
{
	int		i;
	int		quote;
	char	*new_file;
	char	*temp;

	i = 0;
	quote = 0;
	new_file = ft_strdup("");
	//malloc protection
	while (file[i])
	{
		if (file[i] == '$' && file[i + 1] && !quote)
			i = expand_line_helper(file, &new_file, expand, i);
		else
		{
			if (file[i] == 39)
				quote = !quote;
			append_char(&new_file, file, i);
			i++;
		}
	}
	if (!expand->expanded)
	{
		temp = handle_quotes(new_file);
		if (temp)
		{
			new_file = temp;
			temp = NULL;
		}
	}
	return (new_file);
}

