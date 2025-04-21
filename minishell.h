#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <signal.h>
# include <stdbool.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# define MALLOC "minishell: memory allocation failure\n"
# define SYNTAX "minishell: syntax error near unexpected token %s\n"
# define EXPORT "minishell: export: `%s': not a valid identifier\n"
# define ERR_PIPE "minishell: failed to create pipe"
# define ERR_WAITPID "minishell: waitpid failed"
# define ERR_COMMAND "Command '%s' not found\n"
# define ERR_FORK "failed to fork"
# define ERR_NUM "numeric argument required"
# define ERR_ARG "too many arguments"
# define ERR_INVFILE "minishell: %s: No such file or directory\n"
# define ERR_DIR "minishell: %s: Is a directory"
# define ERR_INVPERMS "minishell: %s: Permission denied\n"
# define ERR_FD "failed to return a file descriptor"
# define ERR_CLOSE "failed to close a file descriptor"
# define ERR_EXECVE "minishell: %s: Unknown failure"

extern int	g_signum;
typedef enum s_type
{
	PIPE,
	COMMAND,
	REDIR_INF,
	REDIR_OUTF,
	REDIR_APPEND,
	REDIR_HEREDOC,
}			t_type;

typedef struct s_index
{
	int	i;
	int	j;
}		t_index;

typedef struct s_node
{
	t_type			type;
	char			**cmd;
	char			*file;
	char			*delimiter;
	bool			delimiter_quote;
	struct s_node	*prev;
	struct s_node	*next;
}					t_node;

typedef struct s_exp
{
	bool			expanded;
	bool			no_element;
	int				status;
	char			**new_cmd;
	char			*exp;
	struct s_data	*data;
	struct s_pipes	*my_pipes;
}		t_exp;

typedef struct s_data
{
	char		**envp;
	int			sections_amount;
	char		**sections;
	char		***tokens;
	t_node		*first;
}				t_data;

typedef struct s_pipes
{
	pid_t			*childpids;
	int				*pipes;
	int				pipe_amount;
	int				current_section;
	int				stdinfd;
	int				stdoutfd;
	int				infile_fd;
	int				outfile_fd;
	int				read_end;
	int				write_end;
	int				exit_status;
	char			*command_path;
	char			**paths;
	char			***my_envp;
	struct s_node	*command_node;
}					t_pipes;

//init and input validation
char	**copy_envp(char **envp);
void	init_data(t_data *data, char ***envp);
char	*add_spaces(char *input, t_data *data);
void	update_quote(char c, int *quote);
int		is_missing_pre_space(char *input, int i, int quote);
int		is_missing_post_after_pre_space(char *input, int i);
int		is_missing_post_space(char *input, int i, int quote);
char	*check_pipes(char *line, t_data *data);
void	init_sections(t_data *data, char *line);
void	init_tokens(t_data *data);

//lexing
int		lexer(t_data *data);
int		make_all_redir_nodes(t_data *data, int i);
t_node	*init_new_node(t_data *data, t_node *new_node);
int		set_cmd_node(t_data *data, t_index *index, t_node *new_node);
char	**ft_ms_split(char const *s, char c, int *error);
int		ft_ms_strings(char const *s, char c, int i);
int		ft_ms_checkquote(char const *s, int i, char quote);
char	**ft_ms_freearray(char **array, int j, int *error);

//parsing
void	handle_cmd(t_node *tmp, t_data *data, int status);
void	handle_filename(t_node *tmp, t_data *data, int status);
char	*expand_heredoc(char *line, t_pipes *my_pipes, int fd, int status);
char	**handle_cmd_helper(char **cmd, t_data *data, int status);
char	*handle_filename_helper(char *file, t_data *data, int status);
char	*handle_quotes(char *s, t_data *data, t_exp *expand);
char	*find_envp(t_exp *expand, int i, int new_arg);
void	init_exp(t_exp *exp, int status, t_data *data, t_pipes *my_pipes);
char	*find_exp(char *arg, int *i, int *k, t_data *data);
char	*find_replacer(char *arg, int i, t_exp *expand, int new_arg);
void	append_char(char **new_string, char c, t_exp *expand);
void	append_char_heredoc(char **new_string, char *s, int i,
			t_pipes *my_pipes);
void	append_replacer(char **new_string, char *replacer, int is_freeable,
			t_exp *expand);
int		expand_line_helper(char *file, char **new_file, t_exp *expand, int i);
int		is_redirection(char *token);
int		is_exp_delimiter(char c);
void	handle_quotes_in_expansion(t_exp *expand, int *new_arg, int *arg);
void	count_expandable(char *arg, int *i, int *j);

//cleanup
void	free_array(char **array);
void	free_nodes(t_node *node);
void	free_sections_tokens(t_data *data);
void	fatal_parsing_exit(t_data *data, t_exp *expand, char *input, char *msg);
void	handle_fatal_exit(char *msg, t_pipes *my_pipes,
			t_node *list, char *conversion);

//utils
int		count_elements(char **tokens);
int		is_quote(char *s);
int		is_only_quotes(char *s);
int		is_exp_delimiter(char c);
int		is_char_redirection(char c);
void	signal_handler(int sig);
void	heredoc_signal(int sig);
void	child_signal(int sig);
void	receive_signal(int flag);
void	parent_signal(int sig);
int		heredoc(t_node *curr, t_pipes *my_pipes, char **paths, int status);

//heredoc
void	heredoc_mkdir(char **envp, char **paths);
void	heredoc_rm(char **envp, char **paths);
void	heredoc_rmdir(char **envp, char **paths);

//builtins
void	execute_echo(t_node *node, char ***envp);
void	execute_env(char ***envp);
void	execute_pwd(t_pipes *my_pipes);
void	execute_export(char **cmd, char ***envp, t_pipes *my_pipes);
void	execute_cd(char **cmd, t_pipes *my_pipes);
void	execute_unset(char **cmd, char ***envp, t_pipes *my_pipes);
void	execute_exit(char **cmd, t_pipes *my_pipes);
int		add_existing_envp(char ***new_envp, char **envp,
			t_pipes *my_pipes);
int		add_exported_envp(char ***new_envp, char **cmd, int i,
			t_pipes *my_pipes);
char	**fill_unset_envp(char ***new_envp, char **cmd,
			char **envp, t_pipes *my_pipes);
int		is_valid_to_export(char *arg);
int		find_unset_element(char *arg, char **envp);

//execution
char	**get_paths(char ***envp);
char	*get_absolute_path(char **paths, char *command);
void	open_infile(char *file, t_pipes *my_pipes);
void	set_outfile(char *file, enum s_type redir_type, t_pipes *my_pipes);
int		loop_nodes(t_node *list, char ***envp, int status);
void	free_my_pipes(t_pipes *my_pipes);

#endif
