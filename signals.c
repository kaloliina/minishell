#include "minishell.h"

int	g_signum = 0;

void	init_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);	//prints a newline
		rl_on_new_line();	//informs readline that we are on a new line
		rl_replace_line("", 0);	//if ctrl+c was pressed after typing something, that text will be erased on new line
		rl_redisplay();	//redisplay prompt and input line (which is now empty)
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

void	listen_to_signals(int in_parent)
{
	if (in_parent == 1)
	{
		signal(SIGQUIT, parent_signal);
		signal(SIGINT, parent_signal);
		g_signum = 0;
	}
	else
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}
