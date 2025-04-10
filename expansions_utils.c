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
