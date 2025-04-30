#include "minishell.h"

void	append_char_heredoc(char **new_string, char c,
	t_pipes *my_pipes, t_node *heredoc_node)
{
	char	additive[2];
	char	*temp;

	additive[0] = c;
	additive[1] = '\0';
	temp = *new_string;
	*new_string = ft_strjoin(*new_string, additive);
	if (!*new_string)
	{
		my_pipes->exit_status = 1;
		free (temp);
		handle_fatal_exit(MALLOC, my_pipes, heredoc_node, NULL);
	}
	free (temp);
}

void	append_char(char **new_string, char c, t_exp *expand)
{
	char	additive[2];
	char	*temp;

	additive[0] = c;
	additive[1] = '\0';
	temp = *new_string;
	*new_string = ft_strjoin(temp, additive);
	if (!*new_string)
	{
		free (temp);
		fatal_parsing_exit(expand->parser, expand, NULL, MALLOC);
	}
	free (temp);
}

void	append_replacer(char **new_string, char *replacer,
	int is_freeable, t_exp *expand)
{
	char	*temp;

	temp = *new_string;
	*new_string = ft_strjoin(temp, replacer);
	if (!*new_string)
	{
		free (temp);
		free (replacer);
		fatal_parsing_exit(expand->parser, expand, NULL, MALLOC);
	}
	free (temp);
	if (is_freeable)
		free (replacer);
}
