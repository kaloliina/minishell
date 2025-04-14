#include "minishell.h"

char	*handle_quotes_helper(char *s)
{
	int		i;
	int		j;
	int		s_quote;
	int		d_quote;
	char	*new;

	i = 0;
	j = 0;
	s_quote = 0;
	d_quote = 0;
	new = malloc(ft_strlen(s) + 1);
	//malloc protection
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

void	handle_quotes(t_data *data, char **envp, int status)
{
	int		i;
	t_node	*tmp;
	char	**new_cmd;
	char	*new_line;
	char	*new_file;

	tmp = data->first;
	while (tmp)
	{
		if (tmp->cmd)
		{
			new_cmd = handle_expansion_cmds(tmp->cmd, envp, status);
			if (new_cmd)
			{
				free_array(tmp->cmd);
				tmp->cmd = new_cmd;
				new_cmd = NULL;
			}
		}
		if (tmp->file)
		{
			new_file = handle_expansion_filename(tmp->file, envp, status);
			if (new_file)
			{
				tmp->file = new_file;
				new_file = NULL;
			}
			new_file = handle_quotes_helper(tmp->file);
			if (new_file)
			{
				free (tmp->file);
				tmp->file = new_file;
				new_line = NULL;
			}
		}
		if (tmp->delimiter)
		{
			if (is_quote(tmp->delimiter))
				tmp->delimiter_quote = 1;
			new_line = handle_quotes_helper(tmp->delimiter);
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

char	**copy_envp(char **envp)
{
	char	**my_envp;
	int		i;

	i = count_elements(envp);
	my_envp = malloc(sizeof(char *) * (i + 1));
	if (!my_envp)
	{
		ft_printf(2, "%s\n", MALLOC);
		exit (1);
	}
	i = 0;
	while (envp[i])
	{
		my_envp[i] = ft_strdup(envp[i]);
		if (!my_envp[i])
		{
			ft_printf(2, "%s\n", MALLOC);
			free_array(my_envp);
			exit (1);
		}
		i++;
	}
	my_envp[i] = NULL;
	return (my_envp);
}

int	minishell(char *input, char ***envp, int status)
{
	t_data	data;
	// t_node	*tmp;
	int		k;
	char	*line;

	init_tokens_struct(&data);
	line = add_spaces(input);
	if (!line) //what is this case?
		return (-1);
	init_sections(&data, line);
	init_tokens(&data);
	if (lexer(&data) < 0) //what is this case?
		return (-1);
	handle_quotes(&data, *envp, status);
	// tmp = data.first;
	// while (tmp)
	// {
	// 	printf("type %d file %s delimiter %s", tmp->type, tmp->file, tmp->delimiter);
	// 	k = 0;
	// 	if (tmp->cmd)
	// 	{
	// 		while (tmp->cmd[k])
	// 			printf(" cmd %s", tmp->cmd[k++]);
	// 	}
	// 	printf("\n");
	// 	tmp = tmp->next;
	// }
	g_shell_state = 1;
	status = loop_nodes(data.first, envp, status);
	free_nodes(data.first);
	return (status);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	**my_envp;
	char	**tmp;
	int		status = 0;

	(void)av;
	if (ac != 1)
		return (0);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	my_envp = copy_envp(envp);
	while (1)
	{
		g_shell_state = 0;
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
