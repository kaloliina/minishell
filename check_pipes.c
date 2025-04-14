#include "minishell.h"

static int is_invalid_pipes(char *line, int i)
{
    int pipes;

    pipes = 0;
    if (line[i] == '|')
    {
        i++;
        pipes++;
        while (line[i] == '|' || line[i] == ' ')
        {
            if (line[i] == '|')
                pipes++;
            i++;
        }
        if (line[i] == '\0' || pipes > 1)
            return (1);
    }
    return (0);
}

static int spaces_between_quotes(char *line, int i)
{
    if (line[i] == ' ')
    {
        while (line[i] == ' ')
            i++;
        if (line[i] == '|')
            return (1);
    }
    return (0);
}

static char    *check_pipes_helper(char *line, char **envp, int i, int j)
{
    char    *new_line;

    if (spaces_between_quotes(line, i))
    {
        ft_printf(2, "minishell: syntax error near unexpected token `|'\n");
        return (NULL);
    }
    new_line = ft_substr(line, 0, (j - 1));
    free (line);
    line = new_line;
    new_line = NULL;
    return (line);
}

char	*check_pipes(char *line, char **envp)
{
	char	*new_line;
	char	*temp;
	int		i;
	int		j;

	i = 0;
	while (line[i])
	{
        if (is_invalid_pipes(line, i))
		{
			j = i;
			while (line[i] == '|')
				i++;
			if (line[i] != '\0')
                return (check_pipes_helper(line, envp, i, j));
			else
			{
				temp = readline("> ");
				new_line = ft_strjoin(line, temp);
				free (line);
				free (temp);
				line = new_line;
				new_line = NULL;
				temp = NULL;
                line = check_pipes(line, envp);
            }
		}
		i++;
	}
	return (line);
}
