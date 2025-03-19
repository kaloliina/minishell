#include "minishell.h"

void	minishell(char *input)
{
	t_ast	ast;
	int		k;
	t_node	*tmp;
	char	*line;

	line = add_spaces(input);
	init_sections(&ast, line);
	init_tokens(&ast);
	if (lexer(&ast) < 0)
		return ;
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
