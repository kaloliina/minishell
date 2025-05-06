/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:12:21 by sojala            #+#    #+#             */
/*   Updated: 2025/05/06 12:03:05 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signum = 0;

void	init_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signum = SIGINT;
	}
}

void	heredoc_signal(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		if (close(STDIN_FILENO) < 0)
			print_error(ERR_CLOSE, NULL, NULL);
		g_signum = SIGINT;
	}
}

void	parent_signal(int sig)
{
	if (sig == SIGQUIT)
		write(1, "Quit (core dumped)\n", 20);
	if (sig == SIGINT)
		write(1, "\n", 1);
}

void	listen_to_signals(int in_parent)
{
	if (in_parent == 1)
	{
		signal(SIGQUIT, parent_signal);
		signal(SIGINT, parent_signal);
	}
	else
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}
