#include "minishell.h"

int	is_triple_redirection(char *input, int i)
{
	if (input[i] == '<' && input[i + 1] && input[i + 2])
	{
		if (input[i + 1] == '<' && input[i + 2] == '<')
			return (1);
	}
	else if (input[i] == '>' && input[i + 1] && input[i + 2])
	{
		if (input[i + 1] == '>' && input[i + 2] == '>')
			return (1);
	}
	return (0);
}
