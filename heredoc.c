#include "minishell.h"
char	*expand_heredoc(char *line, char **envp, int fd, int status)
{
	int		i;
	char	*new_line;
	t_exp	expand;

	i = 0;
	init_exp(&expand, status, envp);
	new_line = ft_strdup("");
	//malloc protection
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
			i = expand_line_helper(line, &new_line, &expand, i + 1);
		else
		{
			append_char(&new_line, line, i);
			i++;
		}
	}
	return (new_line);
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
		//malloc protection
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
		//malloc protection
		rmdir_cmd[0] = "rmdir";
		rmdir_cmd[1] = "tmp";
		rmdir_cmd[2] = NULL;
		execve(rmdir_path, rmdir_cmd, envp);
	}
	waitpid(rmdir_pid, &status, 0);
}

static void	heredoc_read(t_node *delimiter_node, t_pipes *my_pipes, int status)
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
			temp = expand_heredoc(line, *my_pipes->my_envp, fd, status);
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
		//malloc protection
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

void	heredoc(t_node *node, t_pipes *my_pipes, char **envp, char **paths, int status)
{
	int		newdir;

	newdir = 0;
	if (chdir("./tmp") == -1)
	{
		if (errno == ENOENT)
		{
			newdir = 1;
			heredoc_mkdir(envp, paths);
		}
	}
	heredoc_read(my_pipes->heredoc_node, my_pipes, status);
	heredoc_rm(envp, paths);
	chdir("..");
	if (newdir)
		heredoc_rmdir(envp, paths);
}
