#include "minishell.h"

int	tokensplit_strings(char const *s, int i)
{
	int		strings;
	int		quote;

	strings = 0;
	quote = 0;
	while (s[i])
	{
		if (!is_whitespace(s[i]))
		{
			strings++;
			while (s[i] && (!is_whitespace(s[i]) || quote))
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
