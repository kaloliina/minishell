#include "minishell.h"

static char	**tokensplit_makearray_helper(char **array, char const *s,
	int *error)
{
	int	i;
	int	j;
	int	start;
	int	len;
	int	quote;

	i = 0;
	j = -1;
	while (s[i])
	{
		quote = 0;
		while (s[i] && is_whitespace(s[i]))
			i++;
		if (!s[i])
			break ;
		start = i;
		len = 0;
		while (s[i] && (!is_whitespace(s[i]) || quote))
			split_setstring(s, &i, &len, &quote);
		array[++j] = ft_substr(s, start, len);
		if (!array[j])
			return (split_freearray(array, j, error));
	}
	array[++j] = NULL;
	return (array);
}

static	char	**tokensplit_makearray(char const *s, int *error)
{
	char	**array;

	array = ft_calloc((tokensplit_strings(s, 0) + 1), sizeof(char *));
	if (!array)
	{
		*error = 1;
		return (NULL);
	}
	array = tokensplit_makearray_helper(array, s, error);
	return (array);
}

char	**split_to_tokens(char const *s, int *error)
{
	if (!s)
		return (NULL);
	*error = 0;
	return (tokensplit_makearray(s, error));
}
