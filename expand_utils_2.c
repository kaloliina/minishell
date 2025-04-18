#include "minishell.h"

void	count_expandable(char *arg, int *i, int *j)
{
	while (!is_exp_delimiter(arg[*i]) && arg[*i])
	{
		(*i)++;
		(*j)++;
	}
}

void	init_exp(t_exp *exp, int status, t_data *data, t_pipes *my_pipes)
{
	exp->expanded = 0;
	exp->no_element = 0;
	exp->status = status;
	exp->new_cmd = NULL;
	exp->exp = NULL;
	if (data)
		exp->data = data;
	else
		exp->data = NULL;
	if (my_pipes)
		exp->my_pipes = my_pipes;
	else
		exp->my_pipes = NULL;
}

void	handle_quotes_in_expansion(t_exp *expand, int *new_arg, int *arg)
{
	char	*temp;

	if (expand->no_element)
		expand->no_element = 0;
	else
	{
		if (!expand->expanded)
		{
			temp = handle_quotes(expand->new_cmd[*new_arg],
					expand->data, expand);
			if (temp)
			{
				expand->new_cmd[*new_arg] = temp;
				temp = NULL;
			}
		}
		(*new_arg)++;
	}
	(*arg)++;
}
