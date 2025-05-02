#include "../minishell.h"

void	handle_redirections(t_pipes *my_pipes)
{
	if (my_pipes->exit_status == 1)
		return ;
	my_pipes->exit_status = 0;
	if (my_pipes->outfile_fd >= 0
		&& (dup2(my_pipes->outfile_fd, STDOUT_FILENO) < 0))
		my_pipes->exit_status = 1;
	if (my_pipes->infile_fd >= 0
		&& (dup2(my_pipes->infile_fd, STDIN_FILENO) < 0))
		my_pipes->exit_status = 1;
	if (my_pipes->current_section != 1 && my_pipes->infile_fd == -1)
	{
		if (dup2(my_pipes->pipes[my_pipes->read_end], STDIN_FILENO) < 0)
			my_pipes->exit_status = 1;
	}
	if ((my_pipes->current_section != my_pipes->pipe_amount + 1)
		&& my_pipes->outfile_fd == -1)
	{
		if (dup2(my_pipes->pipes[my_pipes->write_end], STDOUT_FILENO) < 0)
			my_pipes->exit_status = 1;
	}
	if (my_pipes->exit_status == 1)
		print_error(ERR_FD, NULL, NULL);
}

void	open_infile(char *file, t_pipes *my_pipes)
{
	if (*file == '\0')
	{
		my_pipes->exit_status = 1;
		return ;
	}
	if (my_pipes->exit_status == 0)
		my_pipes->infile_fd = open(file, O_RDONLY);
	if (my_pipes->infile_fd == -1 && my_pipes->exit_status == 0)
	{
		if (errno == ENOENT)
			print_error(ERR_INVFILE, file, NULL);
		else if (errno == EACCES)
			print_error(ERR_INVPERMS, file, NULL);
		else if (errno == EISDIR)
			print_error(ERR_DIR, file, NULL);
		my_pipes->exit_status = 1;
	}
}

void	set_outfile(char *file, enum s_type redir_type, t_pipes *my_pipes)
{
	if (*file == '\0')
	{
		my_pipes->exit_status = 1;
		return ;
	}
	if (redir_type == REDIR_OUTF && my_pipes->exit_status == 0)
		my_pipes->outfile_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (redir_type == REDIR_APPEND && my_pipes->exit_status == 0)
		my_pipes->outfile_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (my_pipes->outfile_fd == -1 && my_pipes->exit_status == 0)
	{
		if (errno == ENOENT)
			print_error(ERR_INVFILE, file, NULL);
		else if (errno == EACCES)
			print_error(ERR_INVPERMS, file, NULL);
		else if (errno == EISDIR)
			print_error(ERR_DIR, file, NULL);
		my_pipes->exit_status = 1;
	}
}
