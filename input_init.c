#include "minishell.h"

static int	count_missing_spaces(char *input)
{
	int	i;
	int	extras;

	i = 0;
	extras = 0;
	while (input[i])
	{
		if (i > 0 && is_char_redirection(input[i]) && input[i - 1] != ' '
			&& !is_char_redirection(input[i - 1]))
			extras++;
		if (is_char_redirection(input[i]) && input[i + 1] && input[i + 1] != ' '
			&& !is_char_redirection(input[i + 1]))
			extras++;
		i++;
	}
	return (extras);
}

static char	*add_spaces_helper(char *line, char *input, int i)
{
	int	j;

	j = 0;
	while (input[i])
	{
		if (i > 0 && is_char_redirection(input[i]) && input[i - 1] != ' '
			&& !is_char_redirection(input[i - 1]))
		{
			line[j++] = ' ';
			line[j++] = input[i];
			if (input[i + 1] && input[i + 1] != ' '
				&& !is_char_redirection(input[i + 1]))
				line[j++] = ' ';
		}
		else if (is_char_redirection(input[i]) && input[i + 1]
			&& input[i + 1] != ' '
			&& !is_char_redirection(input[i + 1]))
		{
			line[j++] = input[i];
			line[j++] = ' ';
		}
		else
			line[j++] = input[i];
		i++;
	}
	line[j] = '\0';
	return (line);
}

char	*add_spaces(char *input)
{
	int		extras;
	char	*line;

	extras = count_missing_spaces(input);
	line = malloc(ft_strlen(input) + extras + 1);
	if (!line)
	{
		ft_putstr_fd("minishell: memory allocation failure\n", 2);
		exit (1);
	}
	return (add_spaces_helper(line, input, 0));
}
