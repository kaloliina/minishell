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
	while (s[i])
	{
		if (s[i] == 34)
		{
			if (d_quote)
				new[j++] = s[i++];
			else
			{
				i++;
				s_quote = !s_quote;
			}
		}
		else if (s[i] == 39)
		{
			if (s_quote)
				new[j++] = s[i++];
			else
			{
				i++;
				d_quote = !d_quote;
			}
		}
		else
			new[j++] = s[i++];
	}
	new[j] = '\0';
	return (new);
}

void	handle_quotes(t_ast *ast)
{
	int		i;
	t_node	*tmp;

	tmp = ast->first;
	while (tmp)
	{
		if (tmp->cmd)
		{
			i = 0;
			while (tmp->cmd[i])
			{
				tmp->cmd[i] = handle_quotes_helper(tmp->cmd[i]);
				i++;
			}
		}
		if (tmp->file)
			tmp->file = handle_quotes_helper(tmp->file);
		if (tmp->delimiter)
			tmp->delimiter = handle_quotes_helper(tmp->delimiter);
		tmp = tmp->next;
	}
}

char	**copy_envp(char **envp)
{
	char	**my_envp;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	my_envp = malloc(sizeof(char *) * (i + 1));
	if (!my_envp)
	{
		ft_printf(2, "minishell: memory allocation failure\n");
		exit (1);
	}
	i = 0;
	while (envp[i])
	{
		my_envp[i] = ft_strdup(envp[i]);
		if (!my_envp[i])
		{
			ft_printf(2, "minishell: memory allocation failure\n");
			free_array(my_envp);
			exit (1);
		}
		i++;
	}
	my_envp[i] = NULL;
	return (my_envp);
}

char	**minishell(char *input, char **envp)
{
	t_ast	ast;
	int		k;
	// t_node	*tmp;
	char	*line;
	char	*temp;
	char	**new_envp;

	init_tokens_struct(&ast);
	line = add_spaces(input);
	if (!line)
		return (NULL);
	temp = handle_expandables(line, envp);
	if (temp)
		line = temp;
	init_sections(&ast, line);
	init_tokens(&ast);
	if (lexer(&ast) < 0)
		return (NULL);
	handle_quotes(&ast);
	// tmp = ast.first;
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
	new_envp = loop_nodes(ast.first, envp);
	free_struct(&ast);
	return (new_envp);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	**my_envp;
	char	**tmp;

	(void)av;
	if (ac != 1)
		return (0);
	signal(SIGINT, signal_handler);
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
		if (input)
			add_history(input);
		tmp = NULL;
		tmp = minishell(input, my_envp);
		my_envp = tmp;
		free (input);
	}
	free_array(my_envp);
	clear_history();
	return (0);
}
