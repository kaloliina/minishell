#include "minishell.h"

/*FRIDAY TO DO:
- quote handling (double quotes, single quotes, spaces in between - what happens when with bash??
+ REDIRS inside quotes!!*/

char	*find_envp(char *exp, char **envp)
{
	int		i;
	int		len;
	char	*replacer;

	i = 0;
	len = ft_strlen(exp);
	replacer = NULL;
	while (envp[i])
	{
		if (!ft_strncmp(exp, envp[i], len))
			replacer = ft_substr(envp[i], len + 1, ((ft_strlen(envp[i]) - len - 1)));
		i++;
	}
	free (exp);
	return (replacer);
}

char	*add_replacer(char *line, char *replacer, int k, int j)
{
	int		i;
	int		l;
	int		m;
	int		len;
	int		quote;
	int		replacer_len;
	char	*new_line;

	i = 0;
	l = 0;
	m = 0;
	quote = 0;
	replacer_len = ft_strlen(replacer);
	len = ft_strlen(line) + replacer_len;
	new_line = malloc(len + 1);
	if (!new_line)
	{
		ft_putstr_fd("minishell: memory allocation failure\n", 2);
		exit (1);
	}
	while (i < (k - 1))
		new_line[l++] = line[i++];
	if (line[i] != '"')
		new_line[l++] = line[i++];
	else
		quote = 2;
	while (replacer_len > 0)
	{
		new_line[l++] = replacer[m++];
		replacer_len--;
	}
	i += (j + 1 + quote);
	while (line[i])
		new_line[l++] = line[i++];
	new_line[l] = '\0';
	free (line);
	return (new_line);
}

char	*handle_expandables(char *line, char **envp)
{
	int		i;
	int		j;
	int		k;
	int		quote;
	char	*exp;
	char	*replacer;

	i = 0;
	k = 0;
	quote = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			j = 0;
			i++;
			k = i;
			while (line[i] <= 'Z' && line[i] >= 'A')
			{
				i++;
				j++;
			}
			if (line[i] == ' ' || (line[i] == '"' && quote))
			{
				exp = ft_substr(line, k, j);
				if (exp && *exp)
				{
					replacer = find_envp(exp, envp);
					if (replacer)
					{
						line = add_replacer(line, replacer, k - 1, j);
						free (replacer);
					}
				}
			}
			i = k;
		}
		else
		{
			if (line[i] == '"')
				quote = !quote;
			i++;
		}
	}
	return (line);
}

void	minishell(char *input, char **envp)
{
	t_ast	ast;
	int		k;
	t_node	*tmp;
	char	*line;
	char	*temp;

	line = add_spaces(input);
	if (!line)
		return ;
	temp = handle_expandables(line, envp);
	if (temp)
		line = temp;
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

int	main(int ac, char **av, char **envp)
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
		minishell(input, envp);
		free (input);
	}
	clear_history();
	return (0);
}
