#include "minishell.h"

void	signal_handler(int sig)
{
	ft_printf(1, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
