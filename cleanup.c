#include "minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (array)
	{
		while (array[i])
			free (array[i++]);
		free (array);
		array = NULL;
	}
}

void	free_nodes(t_node *node)
{
	t_node	*tmp;

	while (node->prev)
		node = node->prev;
	while (node)
	{
		tmp = node;
		node = node->next;
		free_array(tmp->cmd);
		if (tmp->file)
			free (tmp->file);
		if (tmp->delimiter)
			free (tmp->delimiter);
		if (tmp->hd_fd != -1)
		{
			if (close(tmp->hd_fd) < 0)
				print_error(ERR_CLOSE, NULL, NULL);
			tmp->hd_fd = -1;
		}
		free (tmp);
	}
}

void	free_sections_tokens(t_data *parser)
{
	int	i;

	i = 0;
	if (parser->sections)
	{
		while (parser->sections[i])
			free (parser->sections[i++]);
		free (parser->sections);
		parser->sections = NULL;
	}
	i = 0;
	if (parser->tokens)
	{
		while (parser->tokens[i])
			free_array(parser->tokens[i++]);
		free (parser->tokens);
		parser->tokens = NULL;
	}
}

void	fatal_parsing_error(t_data *parser, t_exp *expand,
	char *input, char *msg)
{
	if (msg)
		print_error(msg, NULL, NULL);
	if (input)
		free (input);
	if (parser)
	{
		free_array(parser->envp);
		free_sections_tokens(parser);
		if (parser->first)
			free_nodes(parser->first);
	}
	if (expand)
	{
		if (expand->my_pipes)
			free_my_pipes(expand->my_pipes);
		if (expand->new_cmd)
			free_array(expand->new_cmd);
		if (expand->new_line)
			free (expand->new_line);
		if (expand->exp)
			free (expand->exp);
	}
	if (!msg)
		exit (2);
	exit (1);
}

void	fatal_exec_error(char *msg, t_pipes *my_pipes, t_node *list,
	char *conversion)
{
	int	exit_status;

	exit_status = 1;
	if (msg)
		print_error(msg, conversion, NULL);
	if (my_pipes && my_pipes->hd_dir)
		handle_tmpfile(my_pipes);
	if (!list && my_pipes->command_node)
		free_nodes(my_pipes->command_node);
	else if (!list)
		free_nodes(my_pipes->heredoc_node);
	else
		free_nodes(list);
	if (my_pipes)
	{
		exit_status = my_pipes->exit_status;
		free_array(*my_pipes->my_envp);
		free_my_pipes(my_pipes);
	}
	exit (exit_status);
}
