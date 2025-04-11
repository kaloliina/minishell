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

void	update_quote(char c, int *quote)
{
	if ((c == 34 || c == 39) && !(*quote))
		*quote = c;
	else if ((c == 34 || c == 39) && *quote == c)
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

static char	*add_spaces_helper(char *line, char *input, int i, int j)
{
	int	quote;

	quote = 0;
	while (input[i])
	{
		update_quote(input[i], &quote);
		if (is_missing_pre_space(input, i, quote))
		{
			line[j++] = ' ';
			line[j++] = input[i];
			if (is_missing_post_after_pre_space(input, i))
				line[j++] = ' ';
		}
		else if (is_missing_post_space(input, i, quote))
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
		ft_printf(2, "%s\n", MALLOC);
		exit (1);
	}
	return (add_spaces_helper(line, input, 0, 0));
}
