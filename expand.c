#include "minishell.h"

static char	*handle_quotes_helper(char *new, char *s, int i, int j)
{
	int		s_quote;
	int		d_quote;

	s_quote = 0;
	d_quote = 0;
	while (s[i])
	{
		if ((s[i] == '"' && d_quote) || (s[i] == '\'' && s_quote))
			new[j++] = s[i];
		else if (s[i] == '"' && !d_quote)
			s_quote = !s_quote;
		else if (s[i] == '\'' && !s_quote)
			d_quote = !d_quote;
		else
			new[j++] = s[i];
		i++;
	}
	new[j] = '\0';
	return (new);
}

char	*handle_quotes(char *s, t_data *parser, t_exp *expand)
{
	char	*new;

	new = malloc(ft_strlen(s) + 1);
	if (!new)
		fatal_parsing_error(parser, expand, NULL, ERR_MALLOC);
	return (handle_quotes_helper(new, s, 0, 0));
}

void	handle_cmd(t_node *tmp, t_data *parser, int status)
{
	char	**new_cmd;

	new_cmd = handle_cmd_helper(tmp->cmd, parser, status, 0);
	if (new_cmd)
	{
		free_array(tmp->cmd);
		tmp->cmd = new_cmd;
		new_cmd = NULL;
	}
}

void	handle_filename(t_node *tmp, t_data *parser, int status)
{
	char	*new_file;

	new_file = handle_filename_helper(tmp->file, parser, status);
	if (new_file)
	{
		free (tmp->file);
		tmp->file = new_file;
		new_file = NULL;
	}
}
