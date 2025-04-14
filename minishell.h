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
# define MALLOC "minishell: memory allocation failure"
# define ERR_PIPE "minishell: failed to create pipe"
# define ERR_WAITPID "minishell: waitpid failed"
# define ERR_COMMAND "command not found"
# define ERR_FORK "failed to fork"
# define ERR_NUM "numeric argument required"
# define ERR_INVFILE "No such file or directory"
# define ERR_INVPERMS "Permission denied"
# define ERR_FD "failed to return a file descriptor"
# define ERR_CLOSE "failed to close a file descriptor"

extern int g_shell_state;

typedef enum s_type
{
	PIPE,
	COMMAND,
	REDIR_INF,
	REDIR_OUTF,
	REDIR_APPEND,
	REDIR_HEREDOC,
}			t_type;

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

typedef struct s_data
{
	int			sections_amount;
	char		**sections;
	char		***tokens;
	t_node		*first;
}				t_data;

typedef struct t_pipe
{
	int				*pipes;
	char			*command_path;
	char			**paths;
	char			***my_envp;
	int				current_section;
	int				pipe_amount;
	int				stdinfd;
	int				stdoutfd;
	int				infile_fd;
	int				outfile_fd;
	struct s_node	*command_node;
	struct s_node	*heredoc_node;
	int				read_end;
	int				write_end;
	int				exit_status;
}					t_pipes;

//init
t_node	*init_new_node(t_data *data, t_node *new_node);
void	init_sections(t_data *data, char *line);
void	init_tokens(t_data *data);
void	init_tokens_struct(t_data *data);
int		set_sections(t_data *data, char **tmp_sections);
void	set_last_section(t_data *data, int i, char **tmp_sections);

//lexing
char	*add_spaces(char *input);
int		ft_ms_strings(char const *s, char c, int i);
int		ft_ms_checkquote(char const *s, int i, char quote);
char	**ft_ms_freearray(char **array, int j, int *error);
char	**ft_ms_split(char const *s, char c, int *error);
int		make_node(t_data *data, int i, int j, t_node **first);
int		lexer(t_data *data);

//parsing
int		is_redirection(char *token);
int		is_char_redirection(char c);
int		is_redirection_char(char *s);
void	make_pipe_node(t_data *data, t_node **first);
char	**handle_expansion_cmds(char **cmd, char **envp, int status);
char	*handle_expansion_filename(char *file, char **envp, int status);
void	append_char(char **new_string, char *s, int i);
int		is_exp_delimiter(char c);
char	*add_replacer(char *line, char *replacer, int k, int j);
void	append_replacer(char **new_string, char *replacer);
int		fill_replacer(char *new_line, char *line, int k, int j, int *l);
char	*find_envp(char *exp, char **envp);
char	*handle_quotes_helper(char *s);

//cleanup
void	free_nodes(t_node *node);
void	free_array(char **array);
void	free_sections_tokens(t_data *data);

//utils
int		count_elements(char **tokens);
int		is_quote(char *s);
int		is_only_quotes(char *s);
int		is_exp_delimiter(char c);
void	signal_handler(int sig);
void	heredoc(t_node *node, t_pipes *my_pipes, char **envp, char **paths, int status);

//builtins
void	execute_echo(t_node *node, char ***envp);
void	execute_env(char ***envp);
void	execute_pwd(void);
void	execute_export(char **cmd, char ***envp);
void	execute_cd(char **cmd);
void	execute_unset(char **cmd, char ***envp);
void	execute_exit(char **cmd, t_pipes *my_pipes);
int		fill_new_envp(char ***new_envp, char **envp, char **cmd, int args);
int		add_existing_envp(char **new_envp, char **envp);
int		add_exported_envp(char **new_envp, char **cmd, int i);
int		find_unset_element(char **cmd, char *envp_element);
char	**sort_for_export(char **export, char **envp, int elements);
char	**fill_unset_envp(char **new_envp, char **cmd, char **envp);

//execution
char	**get_paths(char ***envp);
char	*get_absolute_path(char **paths, char *command);
void	open_infile(char *file, t_pipes *my_pipes);
void	set_outfile(char *file, enum s_type redir_type, t_pipes *my_pipes);
int		loop_nodes(t_node *list, char ***envp, int status);
void	free_my_pipes(t_pipes *my_pipes);

#endif
