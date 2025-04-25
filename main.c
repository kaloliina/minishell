#include "minishell.h"

static void	handle_exp_and_quotes(t_data *data, int status)
{
	t_node	*tmp;
	char	*new_line;
	char	*new_file;

	tmp = data->first;
	while (tmp)
	{
		if (tmp->cmd)
			handle_cmd(tmp, data, status);
		if (tmp->file)
			handle_filename(tmp, data, status);
		if (tmp->delimiter)
		{
			if (is_quote(tmp->delimiter))
				tmp->delimiter_quote = 1;
			new_line = handle_quotes(tmp->delimiter, data, NULL);
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
	t_data	data;
	char	*line;
	int		exit_status;

	init_data(&data, envp);
	line = add_spaces(input, &data);
	if (!line) //unclosed quotes
		return (2);
	exit_status = 2;
	line = check_pipes(line, &data, 0, &exit_status);
	if (!line) //only whitespace between two pipes
		return (exit_status);
	init_sections(&data, line);
	init_tokens(&data);
	if (lexer(&data) < 0) //missing filename or delimiter
		return (2);
	handle_exp_and_quotes(&data, status);
	// t_node	*tmp = data.first;
	// while (tmp)
	// {
	// 	printf("type %d file '%s' delimiter %s", tmp->type, tmp->file, tmp->delimiter);
	// 	int	k = 0;
	// 	if (tmp->cmd)
	// 	{
	// 		while (tmp->cmd[k])
	// 			printf(" cmd %s", tmp->cmd[k++]);
	// 	}
	// 	printf("\n");
	// 	tmp = tmp->next;
	// }
	status = loop_nodes(data.first, &data.envp, status);
	*envp = data.envp;
	free_nodes(data.first);
	return (status);
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
		else if (input[0] != '\0')
			status = minishell(input, my_envp, status);
		if (input)
			add_history(input);
		free (input);
	}
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	**my_envp;
	int		status;

	(void)av;
	status = 0;
	if (ac != 1)
		return (0);
	my_envp = copy_envp(envp);
	start_minishell(&my_envp);
	free_array(my_envp);
	clear_history();
	return (0);
}
