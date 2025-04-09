#include "../minishell.h"

//we also might need to check if open fails here..? do we need to exit at this point alrdy
void	open_infile(char *file, t_pipes *my_pipes)
{
	int fd;

	if (access(file, F_OK) != 0)
	{
		ft_printf(2, "%s: %s\n", file, ERR_INVFILE);
		my_pipes->exit_status = 1;
		return ;
	}
	else if (access(file, R_OK) != 0)
	{
		ft_printf(2, "%s: %s\n", file, ERR_INVPERMS);
		my_pipes->exit_status = 1;
		return ;
	}
	my_pipes->infile_fd = open(file, O_RDONLY);
}
//same thing here
void	set_outfile(char *file, enum s_type redir_type, t_pipes *my_pipes)
{
	if (access(file, F_OK) == 0 && access(file, W_OK) != 0)
	{
		ft_printf(2, "%s: %s\n", file, ERR_INVPERMS);
		my_pipes->exit_status = 1;
		return ;
	}
	if (redir_type == REDIR_OUTF)
		my_pipes->outfile_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (redir_type == REDIR_APPEND)
		my_pipes->outfile_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
}
