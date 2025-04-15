#include "minishell.h"

char	*handle_quotes_helper(char *new, char *s, int i, int j)
{
	int		s_quote;
	int		d_quote;

	s_quote = 0;
	d_quote = 0;
	while (s[i])
	{
		if ((s[i] == 34 && d_quote) || (s[i] == 39 && s_quote))
			new[j++] = s[i];
		else if (s[i] == 34 && !d_quote)
			s_quote = !s_quote;
		else if (s[i] == 39 && !s_quote)
			d_quote = !d_quote;
		else
			new[j++] = s[i];
		i++;
	}
	new[j] = '\0';
	return (new);
}

char	*handle_quotes(char *s, t_data *data)
{
	char	*new;

	new = malloc(ft_strlen(s) + 1);
	if (!new)
		fatal_parsing_exit(data, NULL, MALLOC);
	return (handle_quotes_helper(new, s, 0, 0));
}

/*THIS HEREDOC DOESNT WORK BC WE NEED DATA STRUCT TO HANDLE FATAL EXIT - MAYBE COMBINE FATAL EXIT FUNCTIONS
TO INCLUDE BOTH DATA AND MY_PIPES FREEING WHICHEVER EXISTS?*/
char	*expand_heredoc(char *line, t_pipes *my_pipes, int fd, int status)
{
	int		i;
	char	*new_line;
	t_exp	expand;

	i = 0;
	printf("line is here in exp %s\n", line);
	init_exp(&expand, status, NULL);
	new_line = ft_strdup("");
	if (!new_line)
		handle_fatal_exit(MALLOC, my_pipes, my_pipes->command_node);
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
			i = expand_line_helper(line, &new_line, &expand, i + 1);
		else
		{
			append_char_heredoc(&new_line, line, i, my_pipes);
			i++;
		}
	}
	return (new_line);
}

void	handle_cmd(t_node *tmp, t_data *data, int status)
{
	char	**new_cmd;

	new_cmd = handle_cmd_helper(tmp->cmd, data, status);
	if (new_cmd)
	{
		free_array(tmp->cmd);
		tmp->cmd = new_cmd;
		new_cmd = NULL;
	}
}

void	handle_filename(t_node *tmp, t_data *data, int status)
{
	char	*new_file;

	new_file = handle_filename_helper(tmp->file, data, status);
	if (new_file)
	{
		free (tmp->file);
		tmp->file = new_file;
		new_file = NULL;
	}
}
