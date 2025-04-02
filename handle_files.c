#include "../minishell.h"

//remember error cases.. :)
int	open_infile(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	return (fd);
}

int	set_outfile(char *file, enum s_type redir_type)
{
	int	fd;

	if (redir_type == REDIR_OUTF)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (redir_type == REDIR_APPEND)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}
