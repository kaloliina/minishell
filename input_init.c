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
	int	quote;

	j = 0;
	quote = 0;
	while (input[i])
	{
		if ((input[i] == 34 || input[i] == 39) && !quote)
			quote = input[i];
		else if ((input[i] == 34 || input[i] == 39) && quote == input[i])
			quote = 0;
		if (i > 0 && is_char_redirection(input[i]) && input[i - 1] != ' '
			&& !is_char_redirection(input[i - 1]) && !quote)
		{
			line[j++] = ' ';
			line[j++] = input[i];
			if (input[i + 1] && input[i + 1] != ' '
				&& !is_char_redirection(input[i + 1]))
				line[j++] = ' ';
		}
		else if (is_char_redirection(input[i]) && input[i + 1]
			&& input[i + 1] != ' ' && !quote
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

int	check_closing_quote(char *s, int i, int quote)
{
	while (s[i])
	{
		if (s[i] == quote)
			return (i + 1);
		i++;
	}
	return (0);
}

int	check_quotes(char *input)
{
	int	i;
	int	temp;

	i = 0;
	temp = 0;
	while (input[i])
	{
		if (input[i] == 34 || input[i] == 39)
		{
			temp = check_closing_quote(input, i + 1, input[i]);
			if (!temp)
			{
				ft_printf(2, "minishell: unclosed quote(s)\n");
				return (-1);
			}
			else
				i = temp;
		}
		else
			i++;
	}
	return (0);
}

char	*add_spaces(char *input)
{
	int		extras;
	char	*line;

	if (check_quotes(input))
		return (NULL);
	extras = count_missing_spaces(input);
	line = malloc(ft_strlen(input) + extras + 1);
	if (!line)
	{
		ft_printf(2, "minishell: memory allocation failure\n");
		exit (1);
	}
	return (add_spaces_helper(line, input, 0));
}
