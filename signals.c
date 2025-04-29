#include "minishell.h"

int	g_signum = 0;

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
		close (STDIN_FILENO);
		g_signum = SIGINT;
	}
}

void	parent_signal(int sig)
{
	if (sig == SIGQUIT)
	{
		write(1, "Quit (core dumped)\n", 20);
		g_signum = SIGQUIT;
	}
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		g_signum = SIGINT;
	}
}
