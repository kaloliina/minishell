#include "minishell.h"

char	**ft_ms_freearray(char **array, int j, int *error)
{
	int	i;

	i = 0;
	while (i <= j)
	{
		free (array[i]);
		i++;
	}
	free (array);
	*error = 1;
	return (NULL);
}

int	ft_ms_checkquote(char const *s, int i, char quote)
{
	while (s[i])
	{
		if (s[i] == quote)
			return (1);
		i++;
	}
	return (0);
}
int	ft_ms_strings(char const *s, char c, int i)
{
	int		strings;
	int		quote;

	strings = 0;
	quote = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			strings++;
			while (s[i] && (s[i] != c || quote))
			{
				if ((s[i] == '"' || s[i] == '\'') && !quote)
					quote = s[i];
				else if (quote && s[i] == quote)
					quote = 0;
				i++;
			}
		}
		else
			i++;
	}
	return (strings);
}
