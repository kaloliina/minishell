#include "minishell.h"

static int	invalid_exp_line(char *line, char **new_line, int k, t_exp *expand)
{
	int	i;

	i = ft_strlen(line);
	if (k == 1)
		ft_printf(2, ERR_AMB, line);
	return (i);
}

int	expand_line_helper(char *line, char **new_line, t_exp *expand, int i)
{
	int		k;
	char	*replacer;

	expand->exp = find_exp(line, &i, &k, expand);
	if ((expand->exp && *expand->exp) || (line[i] == '?' && line[i - 1] == '$'))
	{
		replacer = find_replacer(line, i, expand, 0);
		if (replacer)
		{
			if (!is_quote(line))
				expand->expanded = 1;
			append_replacer(new_line, replacer, 1, expand);
			if (expand->exp && *expand->exp)
				i = k + ft_strlen(expand->exp);
			else
				i = k + 1;
		}
		else if (line[i] && line[i + 1])
			i = k + ft_strlen(expand->exp);
		else
			i = invalid_exp_line(line, new_line, k, expand);
		free (expand->exp);
	}
	return (i);
}

static char	*handle_quotes_in_line(char *new_line, t_exp *expand)
{
	char	*temp;

	if (!expand->expanded && new_line && *new_line)
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

static char	*expand_line(char *line, t_exp *expand)
{
	int		i;
	int		quote;
	char	*temp;

	i = 0;
	quote = 0;
	expand->new_line = ft_strdup("");
	if (!expand->new_line)
		fatal_parsing_exit(expand->data, NULL, NULL, MALLOC);
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && !quote)
			i = expand_line_helper(line, &expand->new_line, expand, i + 1);
		else
		{
			if (line[i] == '\'')
				quote = !quote;
			append_char(&expand->new_line, line[i], expand);
			i++;
		}
	}
	return (handle_quotes_in_line(expand->new_line, expand));
}

char	*handle_filename_helper(char *line, t_data *data, int status)
{
	t_exp	expand;

	init_exp(&expand, status, data, NULL);
	if (!ft_strchr(line, '$'))
		expand.new_line = handle_quotes(line, data, NULL);
	else
		expand.new_line = expand_line(line, &expand);
	return (expand.new_line);
}
