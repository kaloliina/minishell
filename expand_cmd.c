#include "minishell.h"

static int	expand_cmd_helper(char *arg, t_exp *expand, int new_arg, int i)
{
	int		k;
	char	*exp;
	char	*replacer;

	exp = find_exp(arg, &i, &k);
	if ((exp && *exp) || (arg[i] == '?' && arg[i - 1] == '$'))
	{
		replacer = find_replacer(arg, i, expand, exp);
		if (replacer)
		{
			if (!is_quote(arg))
				expand->expanded = 1;
			append_replacer(&(expand->new_cmd[new_arg]), replacer, 1);
			if (exp && *exp)
				i = k + ft_strlen(exp);
			else
				i = k + 1;
		}
		else if (arg[i] && arg[i + 1])
			i = k + ft_strlen(exp);
		else if (k == 1)
			expand->no_element = 1;
		free (exp);
	}
	return (i);
}

static void	expand_cmd(char **cmd, t_exp *expand, int *arg, int *new_arg)
{
	int		i;
	int		quote;

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
