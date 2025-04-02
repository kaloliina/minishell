#include "minishell.h"
#include <errno.h>

// int		handle_heredoc_exp(char *line, int i, char **envp)
// {
// 	int	j;

// 	j = i;
// 	while (line[i] <= 'A' && line[i] >= 'Z')
// 		j++;
// 	if

// }

char	*heredoc_expandables(char *line, char **envp)
{
	int		i;
	int		j;
	int		k;
	char	*exp;
	char	*replacer;
	char	*new_line;
	char	*new_start;
	char	*new_end;

	i = 0;
	k = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			j = 0;
			i++;
			k = i;
			while (line[i] <= 'Z' && line[i] >= 'A')
			{
				i++;
				j++;
			}
			if (!is_valid_char(line[i]))
			{
				exp = ft_substr(line, k, j);
				if (exp && *exp)
				{
					replacer = find_envp(exp, envp);
					if (replacer)
					{
						new_line = add_replacer(line, replacer, k - 1, j);
						free (line);
						free (replacer);
						line = NULL;
						line = new_line;
						new_line = NULL;
					}
					else
					{
						while (is_valid_char(line[i]))
							i++;
						new_start = ft_substr(line, 0, k - 1);
						new_end = ft_substr(line, i, (ft_strlen(line) - i));
						new_line = ft_strjoin(new_start, new_end);
						free (line);
						line = NULL;
						line = new_line;
					}
				}
			}
			else
			{
				while (is_valid_char(line[i]))
					i++;
				new_start = ft_substr(line, 0, k - 1);
				new_end = ft_substr(line, i, (ft_strlen(line) - i));
				new_line = ft_strjoin(new_start, new_end);
				free (line);
				line = NULL;
				line = new_line;
			}
			i = k;
		}
		else
			i++;
	}
	return (line);
}

static void	heredoc_rm(char **envp, char **paths)
{
	pid_t	rm_pid;
	char	**rm_cmd;
	char	*rm_path;
	int		status;

	rm_pid = fork();
	if (rm_pid == 0)
	{
		rm_path = get_absolute_path(paths, "rm");
		rm_cmd = malloc(sizeof(char *) * 3);
		if (!rm_cmd)
		{
			ft_printf(2, "minishell: memory allocation failure\n");
			exit (1);
		}
		rm_cmd[0] = "rm";
		rm_cmd[1] = "tmpfile";
		rm_cmd[2] = NULL;
		execve(rm_path, rm_cmd, envp);
	}
	waitpid(rm_pid, &status, 0);
}

static void	heredoc_rmdir(char **envp, char **paths)
{
	pid_t	rmdir_pid;
	char	**rmdir_cmd;
	char	*rmdir_path;
	int		status;

	rmdir_pid = fork();
	if (rmdir_pid == 0)
	{
		rmdir_path = get_absolute_path(paths, "rmdir");
		rmdir_cmd = malloc(sizeof(char *) * 3);
		if (!rmdir_cmd)
		{
			ft_printf(2, "minishell: memory allocation failure\n");
			exit (1);
		}
		rmdir_cmd[0] = "rmdir";
		rmdir_cmd[1] = "tmp";
		rmdir_cmd[2] = NULL;
		execve(rmdir_path, rmdir_cmd, envp);
	}
	waitpid(rmdir_pid, &status, 0);
}

static void	heredoc_read(t_node *delimiter_node, t_pipes *my_pipes)
{
	int		fd;
	char	*line;
	char	*temp;

	fd = open("tmpfile", O_CREAT | O_TRUNC | O_WRONLY, 0777);
	while (1)
	{
		line = readline("> ");
		if (!ft_strcmp(line, delimiter_node->delimiter))
			break ;
		if (!delimiter_node->delimiter_quote)
		{
			temp = heredoc_expandables(line, my_pipes->my_envp);
			if (temp)
				line = temp;
		}
		ft_printf(fd, "%s\n", line);
		free (line);
	}
	if (line)
		free (line);
	close (fd);
	fd = open("tmpfile", O_RDONLY);
	my_pipes->stdinfd = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close (fd);
}

static void	heredoc_mkdir(char **envp, char **paths)
{
	pid_t	mkdir_pid;
	int		status;
	char	**mkdir_cmd;
	char	*mkdir_path;

	mkdir_pid = fork();
	if (mkdir_pid == 0)
	{
		mkdir_path = get_absolute_path(paths, "mkdir");
		mkdir_cmd = malloc(sizeof(char *) * 3);
		if (!mkdir_cmd)
		{
			ft_printf(2, "minishell: memory allocation failure\n");
			exit (1);
		}
		mkdir_cmd[0] = "mkdir";
		mkdir_cmd[1] = "tmp";
		mkdir_cmd[2] = NULL;
		execve(mkdir_path, mkdir_cmd, envp);
	}
	waitpid(mkdir_pid, &status, 0);
	if (status == 1)
	{
		//free everything
		exit (1);
	}
	chdir("./tmp");
}

void	heredoc(t_node *node, t_pipes *my_pipes, char **envp, char **paths)
{
	int		fd;
	int		newdir;
	char	*line;

	newdir = 0;
	if (chdir("./tmp") == -1)
	{
		if (errno == ENOENT)
		{
			newdir = 1;
			heredoc_mkdir(envp, paths);
		}
	}
	heredoc_read(my_pipes->heredoc_node, my_pipes);
	heredoc_rm(envp, paths);
	chdir("..");
	if (newdir)
		heredoc_rmdir(envp, paths);
}
