#include "minishell.h"

char	*add_spaces(char *input)
{
	int		i;
	int		j;
	int		extras;
	char	*line;

	i = 0;
	j = 0;
	extras = 0;
	while (input[i])
	{
		if (i > 0 && is_char_redirection(input[i]) && input[i - 1] != ' '
			&& !is_char_redirection(input[i - 1]))
			extras++;
		if (is_char_redirection(input[i]) && input[i + 1] && input[i + 1] != ' '
			&& !is_char_redirection(input[i + 1]))
			extras++;
		i++;
	}
	line = malloc(ft_strlen(input) + extras + 1);
	if (!line)
	{
		ft_putstr_fd("minishell: memory allocation failure\n", 2);
		exit (1);
	}
	i = 0;
	while (input[i])
	{
		if (i > 0 && is_char_redirection(input[i]) && input[i - 1] != ' '
			&& !is_char_redirection(input[i - 1]))
		{
			line[j++] = ' ';
			line[j++] = input[i];
			if (input[i + 1] && input[i + 1] != ' ' && !is_char_redirection(input[i + 1]))
				line[j++] = ' ';
		}
		else if (is_char_redirection(input[i]) && input[i + 1] && input[i + 1] != ' '
			&& !is_char_redirection(input[i + 1]))
		{
			line[j++] = input[i];
			line[j++] = ' ';
		}
		else
			line[j++] = input[i];
		i++;
	}
	line[j] = '\0';
	printf("line %s\n", line);
	return (line);
}

void	minishell(char *input)
{
	t_ast	ast;
	int		i;
	int		k;
	int		error;
	t_node	*tmp;
	char	*line;

	i = 0;
	error = 0;
	line = add_spaces(input);
	init_sections(&ast, line);
	init_tokens(&ast);
	if (lexer(&ast) < 0)
		return ;
	printf("are we here\n");
	tmp = ast.first;
	while (tmp)
	{
		printf("type %d file %s", tmp->type, tmp->file);
		k = 0;
		if (tmp->cmd)
		{
			while (tmp->cmd[k])
				printf(" cmd %s", tmp->cmd[k++]);
		}
		printf("\n");
		tmp = tmp->next;
	}
	free_struct(&ast);
}

int	main(int ac, char **av)
{
	char	*input;

	(void)av;
	if (ac != 1)
		return (0);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	while (1)
	{
		input = readline("minishell > ");
		if (!input || !ft_strcmp(input, "exit"))
		{
			printf("exit\n");
			if (input)
				free (input);
			clear_history();
			return (0);
		}
		if (input)
			add_history(input);
		minishell(input);
		free (input);
	}
	clear_history();
	return (0);
}
