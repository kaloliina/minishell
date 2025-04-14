#include "minishell.h"

//finds the correct line of envp to match the expandable in input
char	*find_envp(char *exp, char **envp)
{
	int		i;
	int		len;
	int		envp_len;
	char	*replacer;

	i = 0;
	len = ft_strlen(exp);
	replacer = NULL;
	while (envp[i])
	{
		envp_len = 0;
		while (envp[i][envp_len] && envp[i][envp_len] != '=')
			envp_len++;
		if (!ft_strncmp(exp, envp[i], envp_len)
			&& !ft_strncmp(exp, envp[i], len))
			replacer = ft_substr(envp[i], len + 1, ((ft_strlen(envp[i]) - len - 1)));
			//malloc protection
		i++;
	}
	return (replacer);
}

char	*handle_quotes(char *s)
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

char	*expand_heredoc(char *line, char **envp, int fd, int status)
{
	int		i;
	char	*new_line;
	t_exp	expand;

	i = 0;
	init_exp(&expand, status, envp);
	new_line = ft_strdup("");
	//malloc protection
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
			i = expand_line_helper(line, &new_line, &expand, i + 1);
		else
		{
			append_char(&new_line, line, i);
			i++;
		}
	}
	return (new_line);
}

void	handle_cmd(t_node *tmp, char **envp, int status)
{
	char	**new_cmd;

	new_cmd = handle_cmd_expansion(tmp->cmd, envp, status);
	if (new_cmd)
	{
		free_array(tmp->cmd);
		tmp->cmd = new_cmd;
		new_cmd = NULL;
	}
}

void	handle_filename(t_node *tmp, char **envp, int status)
{
	char	*new_file;

	new_file = handle_filename_expansion(tmp->file, envp, status);
	if (new_file)
	{
		free (tmp->file);
		tmp->file = new_file;
		new_file = NULL;
	}
}