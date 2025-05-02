#include "minishell.h"

int	is_whitespace(char c)
{
	if ((c <= 13 && c >= 9) || c == 32)
		return (1);
	return (0);
}

void	print_error(char *msg, char *conversion_1, char *conversion_2)
{
	ft_printf(2, "minishell: ");
	if (conversion_1 && !conversion_2)
		ft_printf(2, msg, conversion_1);
	else if (conversion_1 && conversion_2)
		ft_printf(2, msg, conversion_1, conversion_2);
	else
		ft_printf(2, msg);
}
