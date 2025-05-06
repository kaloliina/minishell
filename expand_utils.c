/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:11:20 by sojala            #+#    #+#             */
/*   Updated: 2025/05/06 11:47:27 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_exp(char *arg, int *i, int *k, t_exp *expand)
{
	int		j;
	char	*exp;

	j = 0;
	*k = *i;
	count_expandable(arg, i, &j);
	exp = ft_substr(arg, *k, j);
	if (!exp && expand->parsing)
		fatal_parsing_error(expand->parser, expand, NULL, ERR_MALLOC);
	else if (!exp)
	{
		free (expand->new_line);
		expand->new_line = NULL;
		fatal_exec_error(ERR_MALLOC, expand->my_pipes, NULL, NULL);
	}
	return (exp);
}

char	*find_replacer(char *arg, int i, t_exp *expand)
{
	char	*status;

	if (arg[i] == '?' && arg[i - 1] == '$')
	{
		status = ft_itoa(expand->status);
		if (!status && expand->parsing)
			fatal_parsing_error(expand->parser, expand, NULL, ERR_MALLOC);
		else if (!status)
		{
			free (expand->exp);
			expand->exp = NULL;
			free (expand->new_line);
			expand->new_line = NULL;
			fatal_exec_error(ERR_MALLOC, expand->my_pipes, NULL, NULL);
		}
		return (status);
	}
	else
		return (find_envp(expand, 0));
}

static char	**find_envp_source(t_exp *expand)
{
	if (!expand->parser)
		return (*expand->my_pipes->my_envp);
	else
		return (expand->parser->envp);
}

static void	find_envp_failure(t_exp *expand)
{
	if (expand->parsing)
		fatal_parsing_error(expand->parser, expand, NULL, ERR_MALLOC);
	else
	{
		free (expand->new_line);
		expand->new_line = NULL;
		if (expand->exp)
		{
			free (expand->exp);
			expand->exp = NULL;
		}
		fatal_exec_error(ERR_MALLOC, expand->my_pipes, NULL, NULL);
	}
}

char	*find_envp(t_exp *expand, int i)
{
	int		len;
	int		envp_len;
	char	*replacer;
	char	**envp;

	len = ft_strlen(expand->exp);
	replacer = NULL;
	envp = find_envp_source(expand);
	while (envp[i])
	{
		envp_len = 0;
		while (envp[i][envp_len] && envp[i][envp_len] != '=')
			envp_len++;
		if (!ft_strncmp(expand->exp, envp[i], envp_len)
			&& !ft_strncmp(expand->exp, envp[i], len))
		{
			replacer = ft_substr(envp[i], len + 1,
					((ft_strlen(envp[i]) - len - 1)));
			if (!replacer)
				find_envp_failure(expand);
		}
		i++;
	}
	return (replacer);
}
