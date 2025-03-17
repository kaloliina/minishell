/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testi.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:20:26 by khiidenh          #+#    #+#             */
/*   Updated: 2025/03/12 17:27:45 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//tokenization, space as the delimiter and then 2D array is separated by a pipe?
# include "libft/includes/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


// char **create_tokens(char **token_list, char *token, int *token_count, int length)
// {
// 	char **new_list;
// 	char *new_token;


// 	new_list = realloc(token_list, (*token_count + 2) * sizeof(char*));
// 	new_token = ft_substr(token, 0, length);
// 	new_list[*token_count] = new_token;
// 	(*token_count)++;
// 	new_list[*token_count] = NULL;
// 	return (new_list);
// }

char **create_tokens(char **token_list, char *token, int length)
{
//this needs to have malloc checks and of course tokenizer also needs to have them.
	int token_count = 0;
	if (token_list == NULL)
		token_count = 0;
	else
	{
		while (token_list[token_count] != NULL)
		token_count++;
	}
	token_list = realloc(token_list, (token_count + 2) * sizeof(char*));
	token_list[token_count] = ft_substr(token, 0, length);
	printf("%s", token_list[token_count]);
	token_count++;
	token_list[token_count] = NULL;
	return (token_list);
}

char **tokenizer(char *arg)
{
//this MIGHT need to be split into another function to handle "special cases such as redirections and inquotes.
	int i = 0;
	int start;
	char in_quotes = 0;
	char **token_list = NULL;

	while (arg[i] != '\0')
	{
		while (arg[i] == ' ')
			i++;
		start = i;
		if (arg[i] == '\0')
			break;
		if ((arg[i] == '>' || arg[i] == '<') && (arg[i + 1] == '>' || arg[i + 1] == '<'))
				i++;
		if (arg[i] == 34 || arg[i] == 39)
		{
			in_quotes = arg[i];
			i++;
			while (arg[i] != in_quotes)
				i++;
		}
		if (arg[i] == '>' || arg[i] == '<' || arg[i] == in_quotes)
		{
			token_list = create_tokens(token_list, &arg[start], i - start + 1);
			i++;
			continue;
		}
		while (arg[i] != '\0' && arg[i] != ' ' && arg[i] != '\'' && arg[i] != '<' && arg[i] != '>')
			i++;
		token_list = create_tokens(token_list, &arg[start], i - start);
	}
	return (token_list);
}


int main(int argc, char *argv[])
{
	char *input;
	char **args;
	char ***commands;
	int i = 0;
	int j = 0;
	while ((input = readline("minishell> ")) != NULL)
	{
		if (*input)
		{
			add_history(input);
		}
		printf("You entered: %s\n", input);
		args = ft_split(input, '|');
		while (args[i] != NULL)
		{
			i++;
		}
		commands = malloc(i * (sizeof(char**) + 1));
		commands[i] = NULL;
		i = 0;
		while (args[i] != NULL)
		{
			j = 0;
			commands[i] = tokenizer(args[i]);
			while (commands[i][j] != NULL)
			{
				printf("[%s]", commands[i][j]);
				j++;
			}
			i++;
		}
		j = 0;
	}
}

// char **create_tokens(char **token_list, char *token, int *token_count, int length)
// {
// //token count could be calculated here
// 	token_list = realloc(token_list, (*token_count + 2) * sizeof(char*));
// 	token_list[*token_count] = ft_substr(token, 0, length);
// 	printf("%s", token_list[*token_count]);
// 	(*token_count)++;
// 	token_list[*token_count] = NULL;
// 	return (token_list);
// }

// char **tokenizer(char *arg)
// {
// //i could be removed
// //maybe some of the parts could be simplified
// 	int i = 0;
// 	int token_count = 0;
// 	int start;
// 	char in_quotes = 0;
// 	char **token_list = NULL;

// 	while (arg[i] != '\0')
// 	{
// 		while (arg[i] == ' ')
// 			i++;
// 		start = i;
// 		if (arg[i] == '\0')
// 			break;
// 		if (arg[i] == '>' || arg[i] == '<')
// 		{
// 			if (arg[i + 1] == arg[i])
// 				i++;
// 			token_list = create_tokens(token_list, &arg[start], &token_count, i - start + 1);
// 			i++;
// 			continue;
// 		}
// 		if (arg[i] == 34 || arg[i] == 39)
// 		{
// 			in_quotes = arg[i];
// 			i++;
// 			while (arg[i] != in_quotes)
// 				i++;
// 			token_list = create_tokens(token_list, &arg[start], &token_count, i - start + 1);
// 			i++;
// 			continue;
// 		}
// 		while (arg[i] != '\0' && arg[i] != ' ' && arg[i] != '\'' && arg[i] != '<' && arg[i] != '>')
// 			i++;
// 		token_list = create_tokens(token_list, &arg[start], &token_count, i - start);
// 	}
// 	return (token_list);
// }
