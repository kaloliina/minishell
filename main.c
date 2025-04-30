#include "minishell.h"

static void	handle_exp_and_quotes(t_data *parser, int status)
{
	t_node	*tmp;
	char	*new_line;

	tmp = parser->first;
	while (tmp)
	{
		if (tmp->cmd)
			handle_cmd(tmp, parser, status);
		if (tmp->file)
			handle_filename(tmp, parser, status);
		if (tmp->delimiter)
		{
			if (is_quote(tmp->delimiter))
				tmp->delimiter_quote = 1;
			new_line = handle_quotes(tmp->delimiter, parser, NULL);
			if (new_line)
			{
				free (tmp->delimiter);
				tmp->delimiter = new_line;
				new_line = NULL;
			}
		}
		tmp = tmp->next;
	}
}

static int	minishell(char *input, char ***envp, int status)
{
	t_data	parser;
	char	*line;
	int		exit_status;

	init_parser(&parser, envp);
	if (is_unclosed_quote(input) || is_only_pipes(input))
		return (2);
	line = add_spaces(input, &parser);
	exit_status = 2;
	line = check_pipes(line, &parser, 0, &exit_status);
	if (!line) //only whitespace between two pipes
		return (exit_status);
	init_sections(&parser, line);
	init_tokens(&parser);
	if (lexer(&parser) < 0) //missing filename or delimiter
		return (2);
	handle_exp_and_quotes(&parser, status);
	status = begin_execution(parser.first, &parser.envp, status);
	*envp = parser.envp;
	free_nodes(parser.first);
	return (status);
}

static int	is_only_space(char *input)
{
	int	i;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	if (input[i] == '\0')
		return (1);
	return (0);
}

static void	start_minishell(char ***my_envp)
{
	int		status;
	char	*input;

	status = 0;
	while (1)
	{
		signal(SIGINT, init_signal_handler);
		signal(SIGQUIT, SIG_IGN);
		input = readline("minishell > ");
		if (g_signum == SIGINT)
		{
			status = 128 + g_signum;
			g_signum = 0;
		}
		if (!input)
		{
			ft_printf(1, "exit\n");
			return ;
		}
		else if (input[0] != '\0' && !is_only_space(input))
			status = minishell(input, my_envp, status);
		if (input)
			add_history(input);
		free (input);
	}
}

int	main(int ac, char **av, char **envp)
{
	char	**my_envp;
	int		status;

	(void)av;
	status = 0;
	if (ac != 1)
	{
		ft_printf(2, "minishell: expected format: <./minishell>\n");
		return (0);
	}
	my_envp = copy_envp(envp);
	start_minishell(&my_envp);
	free_array(my_envp);
	return (0);
}
