#include "minishell.h"

static int	expand_cmd_helper(char *arg, t_exp *expand, int new_arg, int i)
{
	int		k;
	char	*replacer;

	expand->exp = find_exp(arg, &i, &k, expand->data);
	if ((expand->exp && *expand->exp) || (arg[i] == '?' && arg[i - 1] == '$'))
	{
		replacer = find_replacer(arg, i, expand, new_arg);
		if (replacer)
		{
			if (!is_quote(arg))
				expand->expanded = 1;
			append_replacer(&(expand->new_cmd[new_arg]), replacer, 1, expand);
			if (expand->exp && *expand->exp)
				i = k + ft_strlen(expand->exp);
			else
				i = k + 1;
		}
		else if (arg[i] && arg[i + 1])
			i = k + ft_strlen(expand->exp);
		else if (k == 1)
			expand->no_element = 1;
		free (expand->exp);
	}
	return (i);
}

static int	is_only_dollar(char *arg, int i)
{
	if (arg[i + 1] == '$' && ((arg[i] == '"' && arg[i + 2] == '"')
			|| (arg[i] == '\'' && arg[i + 2] == '\'')))
		return (1);
	return (0);
}

static int	is_expandable(char *arg, int i, int quote)
{
	if (arg[i] == '$' && arg[i + 1]
		&& !quote && arg[i + 1] != ' ')
		return (1);
	return (0);
}

static void	expand_cmd(char **cmd, t_exp *expand, int *arg, int *new_arg)
{
	int		i;
	int		quote;

	i = 0;
	quote = 0;
	expand->new_cmd[*new_arg] = ft_strdup("");
	if (!expand->new_cmd[*new_arg])
		fatal_parsing_exit(expand->data, expand, NULL, MALLOC);
	while (cmd[*arg][i])
	{
		if (is_only_dollar(cmd[*arg], i))
		{
			append_char(&expand->new_cmd[*new_arg], '$', expand);
			i += 3;
		}
		else if (is_expandable(cmd[*arg], i, quote))
			i = expand_cmd_helper(cmd[*arg], expand, *new_arg, i + 1);
		else
		{
			update_single_quote(cmd[*arg][i], &quote);
			append_char(&expand->new_cmd[*new_arg], cmd[*arg][i], expand);
			i++;
		}
	}
	handle_quotes_in_expansion(expand, new_arg, arg);
}

char	**handle_cmd_helper(char **cmd, t_data *data, int status)
{
	int		arg;
	int		new_arg;
	t_exp	expand;

	arg = 0;
	new_arg = 0;
	init_exp(&expand, status, data, NULL);
	expand.new_cmd = malloc(sizeof(char *) * (count_elements(cmd) + 1));
	if (!expand.new_cmd)
		fatal_parsing_exit(data, &expand, NULL, MALLOC);
	expand.new_cmd[0] = NULL;
	while (cmd[arg])
	{
		if (!ft_strchr(cmd[arg], '$'))
			expand.new_cmd[new_arg++]
				= handle_quotes(cmd[arg++], data, &expand);
		else
			expand_cmd(cmd, &expand, &arg, &new_arg);
	}
	if (!new_arg)
		expand.new_cmd[new_arg++] = ft_strdup("");
	expand.new_cmd[new_arg] = NULL;
	return (expand.new_cmd);
}
