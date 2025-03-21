#include "minishell.h"

static char	*ft_ms_makestring(char const *s, int start, int len)
{
	// if ((s[start] == 39 && s[i - 1] == 39)
	// 	|| (s[start] == 34 && s[i - 1] == 34))
	// 	return (ft_substr(s, start + 1, len - 2));
	// else
	return (ft_substr(s, start, len));
}

static void	ft_ms_setstring(char const *s, int *i, int *len, int *quote)
{
	if ((s[*i] == 39 && *quote == 39) || (s[*i] == 34 && *quote == 34))
		*quote = 0;
	if (!(*quote) && (s[*i] == 39 || s[*i] == 34)
		&& ft_ms_checkquote(s, *i + 1, s[*i]))
		*quote = s[*i];
	*i = *i + 1;
	*len = *len + 1;
}

static char	**ft_ms_makearray_helper(char **array, char const *s,
	char c, int *error)
{
	int	i;
	int	j;
	int	start;
	int	len;
	int	quote;

	i = 0;
	j = -1;
	quote = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (!s[i])
			break ;
		start = i;
		len = 0;
		while (s[i] && (s[i] != c || quote))
			ft_ms_setstring(s, &i, &len, &quote);
		array[++j] = ft_ms_makestring(s, start, len);
		if (!array[j])
			return (ft_ms_freearray(array, j, error));
	}
	array[++j] = NULL;
	return (array);
}

static	char	**ft_ms_makearray(char const *s, char c, int *error)
{
	char	**array;

	array = malloc((ft_ms_strings(s, c, 0) + 1) * sizeof(char *));
	if (!array)
	{
		*error = 1;
		return (NULL);
	}
	array = ft_ms_makearray_helper(array, s, c, error);
	return (array);
}

char	**ft_ms_split(char const *s, char c, int *error)
{
	if (!s)
		return (NULL);
	*error = 0;
	return (ft_ms_makearray(s, c, error));
}
