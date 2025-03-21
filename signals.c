#include "minishell.h"

volatile sig_atomic_t	g_received = 0;

void	signal_handler(int sig)
{
	g_received = sig;
	// if (g_received == 3)
	// {

	// }
	// else
	// {
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	// }
}
