#include "minishell.h"

void	update_quote(char c, int *quote)
{
	if ((c == '"' || c == '\'') && !(*quote))
		*quote = c;
	else if ((c == '"' || c == '\'') && *quote == c)
		*quote = 0;
}

int	is_missing_pre_space(char *input, int i, int quote)
{
	if (i > 0 && is_char_redirection(input[i]) && input[i - 1] != ' '
		&& !is_char_redirection(input[i - 1]) && !quote)
		return (1);
	return (0);
}

int	is_missing_post_after_pre_space(char *input, int i)
{
	if (input[i + 1] && input[i + 1] != ' '
		&& !is_char_redirection(input[i + 1]))
		return (1);
	return (0);
}

int	is_missing_post_space(char *input, int i, int quote)
{
	if (is_char_redirection(input[i]) && input[i + 1]
		&& input[i + 1] != ' ' && !quote
		&& !is_char_redirection(input[i + 1]))
		return (1);
	return (0);
}
