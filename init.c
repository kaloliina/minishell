#include "minishell.h"

t_node	*init_new_node(t_data *parser, t_node *new_node)
{
	new_node = ft_calloc(1, sizeof(t_node));
	if (!new_node)
		fatal_parsing_error(parser, NULL, NULL, ERR_MALLOC);
	new_node->cmd = NULL;
	new_node->file = NULL;
	new_node->delimiter = NULL;
	new_node->delimiter_quote = 0;
	new_node->hd_fd = -1;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	init_sections(t_data *parser, char *line)
{
	int	error;

	error = 0;
	parser->sections = split_to_sections(line, '|', &error);
	if (!parser->sections && error)
		fatal_parsing_error(parser, NULL, line, ERR_MALLOC);
	parser->sections_amount = count_elements(parser->sections);
	free (line);
}

static char	**init_only_quotes_section(t_data *parser, int i)
{
	parser->tokens[i] = ft_calloc(2, sizeof(char *));
	if (!parser->tokens[i])
		fatal_parsing_error(parser, NULL, NULL, ERR_MALLOC);
	parser->tokens[i][0] = ft_strdup(parser->sections[i]);
	if (!parser->tokens[i][0])
	{
		parser->tokens[i + 1] = NULL;
		fatal_parsing_error(parser, NULL, NULL, ERR_MALLOC);
	}
	parser->tokens[i][1] = NULL;
	return (parser->tokens[i]);
}

void	init_tokens(t_data *parser)
{
	int		i;
	int		error;

	parser->tokens = ft_calloc((parser->sections_amount + 1), sizeof(char **));
	if (!parser->tokens)
		fatal_parsing_error(parser, NULL, NULL, ERR_MALLOC);
	i = 0;
	error = 0;
	while (parser->sections[i])
	{
		if (!is_only_quotes(parser->sections[i]))
		{
			parser->tokens[i] = split_to_tokens(parser->sections[i], &error);
			if (!parser->tokens[i] && error)
				fatal_parsing_error(parser, NULL, NULL, ERR_MALLOC);
		}
		else
			parser->tokens[i] = init_only_quotes_section(parser, i);
		i++;
	}
	parser->tokens[i] = NULL;
}

void	init_parser(t_data *parser, char ***envp)
{
	parser->envp = *envp;
	parser->first = NULL;
	parser->sections = NULL;
	parser->sections_amount = 0;
	parser->tokens = NULL;
}
