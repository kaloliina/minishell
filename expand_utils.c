#include "minishell.h"

char	*find_exp(char *arg, int *i, int *k, t_exp *expand)
{
	int		j;
	char	*exp;

	j = 0;
	*k = *i;
	count_expandable(arg, i, &j);
	exp = ft_substr(arg, *k, j);
	if (!exp)
		fatal_parsing_exit(expand->data, expand, NULL, MALLOC);
	return (exp);
}

char	*find_replacer(char *arg, int i, t_exp *expand)
{
	if (arg[i] == '?' && arg[i - 1] == '$')
		return (ft_itoa(expand->status));
	else
		return (find_envp(expand, 0));
}

static char	**find_envp_source(t_exp *expand)
{
	if (!expand->data)
		return (*expand->my_pipes->my_envp);
	else
		return (expand->data->envp);
}

static void	find_envp_failure(t_exp *expand)
{
	if (expand->parsing)
	{
		free (expand->new_line);
		fatal_parsing_exit(expand->data, expand, NULL, MALLOC);
	}
	else
	{
		free (expand->new_line);
		if (expand->exp)
			free (expand->exp);
		handle_fatal_exit(MALLOC, expand->my_pipes, NULL, NULL);
	}
}

char	*find_envp(t_exp *expand, int i)
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
				find_envp_failure(expand);
		}
		i++;
	}
	return (replacer);
}
