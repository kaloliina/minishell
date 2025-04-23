#include "../minishell.h"

void	open_infile(char *file, t_pipes *my_pipes)
{
	if (my_pipes->exit_status == 0)
		my_pipes->infile_fd = open(file, O_RDONLY);
	if (my_pipes->infile_fd == -1 && my_pipes->exit_status == 0)
	{
		if (errno == ENOENT)
			ft_printf(2, ERR_INVFILE, file);
		else if (errno == EACCES)
			ft_printf(2, ERR_INVPERMS, file);
		else if (errno == EISDIR)
			ft_printf(2, ERR_DIR, file);
		my_pipes->exit_status = 1;
	}
}

void	set_outfile(char *file, enum s_type redir_type, t_pipes *my_pipes)
{
	if (redir_type == REDIR_OUTF && my_pipes->exit_status == 0)
		my_pipes->outfile_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (redir_type == REDIR_APPEND && my_pipes->exit_status == 0)
		my_pipes->outfile_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (my_pipes->outfile_fd == -1 && my_pipes->exit_status == 0)
	{
		if (errno == ENOENT)
			ft_printf(2, ERR_INVFILE, file);
		else if (errno == EACCES)
			ft_printf(2, ERR_INVPERMS, file);
		else if (errno == EISDIR)
			ft_printf(2, ERR_DIR, file);
		my_pipes->exit_status = 1;
	}
}
