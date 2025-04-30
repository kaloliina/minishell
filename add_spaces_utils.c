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
	if (i > 0 && is_char_redirection(input[i]) && !is_whitespace(input[i - 1])
		&& !is_char_redirection(input[i - 1]) && !quote)
		return (1);
	return (0);
}

int	is_missing_post_after_pre_space(char *input, int i)
{
	if (input[i + 1] && !is_whitespace(input[i + 1])
		&& !is_char_redirection(input[i + 1]))
		return (1);
	return (0);
}

int	is_missing_post_space(char *input, int i, int quote)
{
	if (is_char_redirection(input[i]) && input[i + 1]
		&& !is_whitespace(input[i + 1]) && !quote
		&& !is_char_redirection(input[i + 1]))
		return (1);
	if (i > 0 && is_char_redirection(input[i]) && input[i + 1]
		&& input[i - 1] == input[i] && input[i + 1] == input[i])
		return (1);
	return (0);
}

int	is_only_pipes(char *input)
{
	int	i;

	i = 0;
	while (is_whitespace(input[i + 1]))
		i++;
	if (input[i] != '|')
		return (0);
	ft_printf(2, "minishell: ");
	if (input[i] == '|' && input[i + 1] == '|')
		ft_printf(2, SYNTAX, "||");
	else
		ft_printf(2, SYNTAX, "|");
	return (1);
}
