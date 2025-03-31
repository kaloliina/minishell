#include "minishell.h"

static char	*find_envp(char *exp, char **envp)
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

static char	*add_replacer(char *line, char *replacer, int k, int j)
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
	len = ft_strlen(line) + replacer_len;
	new_line = malloc(len + 1);
	if (!new_line)
	{
		ft_putstr_fd("minishell: memory allocation failure\n", 2);
		exit (1);
	}
	while (i < (k - 1))
		new_line[l++] = line[i++];
	if (line[i] != '"')
		new_line[l++] = line[i++];
	else
		quote = 2;
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

char	*handle_expandables(char *line, char **envp)
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
		if (line[i] == '$')
		{
			j = 0;
			i++;
			k = i;
			while (line[i] <= 'Z' && line[i] >= 'A')
			{
				i++;
				j++;
			}
			if (line[i] == ' '  || line[i] == '\0' || (line[i] == '"' && quote))
			{
				exp = ft_substr(line, k, j);
				if (exp && *exp)
				{
					replacer = find_envp(exp, envp);
					if (replacer)
					{
						new_line = add_replacer(line, replacer, k - 1, j);
						free (line);
						free (replacer);
						line = NULL;
						line = new_line;
						new_line = NULL;
					}
				}
			}
			i = k;
		}
		else
		{
			if (line[i] == '"')
				quote = !quote;
			i++;
		}
	}
	return (line);
}
