#include "minishell.h"

void	handle_exp_and_quotes(t_data *data, char **envp, int status)
{
	t_node	*tmp;
	char	*new_line;
	char	*new_file;

	tmp = data->first;
	while (tmp)
	{
		if (tmp->cmd)
			handle_cmd(tmp, envp, status);
		if (tmp->file)
			handle_filename(tmp, envp, status);
		if (tmp->delimiter)
		{
			if (is_quote(tmp->delimiter))
				tmp->delimiter_quote = 1;
			new_line = handle_quotes(tmp->delimiter);
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

int	minishell(char *input, char ***envp, int status)
{
	t_data	data;
	t_node	*tmp;
	int		k;
	char	*line;

	init_data(&data);
	line = add_spaces(input, *envp);
	if (!line) //unclosed quotes
		return (-1);
	line = check_pipes(line, *envp);
	if (!line)
		return (-1);
	init_sections(&data, line);
	init_tokens(&data);
	if (lexer(&data) < 0) //missing filename or delimiter
		return (-1);
	handle_exp_and_quotes(&data, *envp, status);
	tmp = data.first;
	while (tmp)
	{
		printf("type %d file %s delimiter %s", tmp->type, tmp->file, tmp->delimiter);
		k = 0;
		if (tmp->cmd)
		{
			while (tmp->cmd[k])
				printf(" cmd %s", tmp->cmd[k++]);
		}
		printf("\n");
		tmp = tmp->next;
	}
	status = loop_nodes(data.first, envp, status);
	free_nodes(data.first);
	return (status);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	**my_envp;
	int		status = 0;

	(void)av;
	if (ac != 1)
		return (0);
	//signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	my_envp = copy_envp(envp);
	while (1)
	{
		input = readline("minishell > ");
		if (!input || !ft_strcmp(input, "exit"))
		{
			ft_printf(1, "exit\n");
			if (input)
				free (input);
			free_array(my_envp);
			clear_history();
			return (0);
		}
		else if (input[0] != '\0')
			status = minishell(input, &my_envp, status);
		if (input)
			add_history(input);
		free (input);
	}
	free_array(my_envp);
	clear_history();
	return (0);
}
