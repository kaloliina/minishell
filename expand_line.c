#include "minishell.h"

static int	invalid_exp_line(char *file, char **new_file, int k, t_exp *expand)
{
	int	i;

	i = ft_strlen(file);
	if (k == 1)
		append_replacer(new_file, file, 0, expand);
	return (i);
}

int	expand_line_helper(char *file, char **new_file, t_exp *expand, int i)
{
	int		k;
	char	*replacer;

	expand->exp = find_exp(file, &i, &k, expand->data);
	if ((expand->exp && *expand->exp) || (file[i] == '?' && file[i - 1] == '$'))
	{
		replacer = find_replacer(file, i, expand, 0);
		if (replacer)
		{
			if (!is_quote(file))
				expand->expanded = 1;
			append_replacer(new_file, replacer, 1, expand);
			if (expand->exp && *expand->exp)
				i = k + ft_strlen(expand->exp);
			else
				i = k + 1;
		}
		else if (file[i] && file[i + 1])
			i = k + ft_strlen(expand->exp);
		else
			i = invalid_exp_line(file, new_file, k, expand);
		free (expand->exp);
	}
	return (i);
}

static char	*handle_quotes_in_line(char *new_line, t_exp *expand)
{
	char	*temp;

	if (!expand->expanded)
	{
		temp = handle_quotes(new_line, expand->data, expand);
		if (temp)
		{
			new_line = temp;
			temp = NULL;
		}
	}
	return (new_line);
}

static char	*expand_line(char *file, t_exp *expand)
{
	int		i;
	int		quote;
	char	*new_line;
	char	*temp;

	i = 0;
	quote = 0;
	new_line = ft_strdup("");
	if (!new_line)
		fatal_parsing_exit(expand->data, NULL, NULL, MALLOC);
	while (file[i])
	{
		if (file[i] == '$' && file[i + 1] && !quote)
			i = expand_line_helper(file, &new_line, expand, i + 1);
		else
		{
			if (file[i] == '\'')
				quote = !quote;
			append_char(&new_line, file[i], expand);
			i++;
		}
	}
	return (handle_quotes_in_line(new_line, expand));
}

char	*handle_filename_helper(char *file, t_data *data, int status)
{
	char	*new_file;
	t_exp	expand;

	init_exp(&expand, status, data, NULL);
	if (!ft_strchr(file, '$'))
		new_file = handle_quotes(file, data, NULL);
	else
		new_file = expand_line(file, &expand);
	return (new_file);
}
