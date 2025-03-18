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

int	ft_ms_strings(char const *s, char c)
{
	int		i;
	int		strings;
	char	quote;

	i = 0;
	strings = 0;
	while (s[i])
	{
		if ((s[i] == 39 || s[i] == 34) && s[i - 1] == c
			&& ft_ms_checkquote(s, i + 1, s[i]))
		{
			quote = s[i];
			strings++;
			i++;
			while (s[i] && s[i] != quote)
				i++;
			i++;
		}
		else if (s[i] != c)
		{
			strings++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (strings);
}
