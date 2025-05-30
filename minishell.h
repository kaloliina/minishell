/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:12:15 by sojala            #+#    #+#             */
/*   Updated: 2025/05/06 17:58:58 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <errno.h>
# define ERR_MALLOC "memory allocation failure\n"
# define ERR_SYNTAX "syntax error near unexpected token `%s'\n"
# define ERR_EXPORT "export: `%s': not a valid identifier\n"
# define ERR_HD "warning: here-document delimited by end-of-file "
# define ERR_HD_DLM "(wanted `%s')\n"
# define ERR_PIPE "pipe: Too many open files\n"
# define ERR_WAITPID "waitpid: No child processes\n"
# define ERR_COMMAND "%s: command not found\n"
# define ERR_FORK "fork: Resource temporarily unavailable\n"
# define ERR_NUM "exit: %s: numeric argument required\n"
# define ERR_ARG "too many arguments\n"
# define ERR_AMB "%s: ambiguous redirect\n"
# define ERR_INVFILE "%s: No such file or directory\n"
# define ERR_DIR "%s: Is a directory\n"
# define ERR_INVPERMS "%s: Permission denied\n"
# define ERR_FD "dup: Bad file descriptor\n"
# define ERR_CLOSE "close: Bad file descriptor\n"
# define ERR_EXECVE "%s: Unknown failure\n"
# define ERR_EOF "syntax error: unexpected end of file\n"
# define ERR_FORMAT "%s: cannot execute binary file: Exec format error\n"
# define ERR_UNLINK "unlink: failed to remove file (minishell_tmpfile)\n"

extern volatile sig_atomic_t	g_signum;

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

typedef struct s_exp
{
	bool			expanded;
	bool			no_element;
	bool			parsing;
	int				status;
	char			**new_cmd;
	char			*new_line;
	char			*exp;
	char			*expansion;
	struct s_data	*parser;
	struct s_pipes	*my_pipes;
}		t_exp;

typedef struct s_node
{
	t_type			type;
	char			**cmd;
	char			*file;
	char			*delimiter;
	bool			delimiter_quote;
	int				hd_fd;
	struct s_node	*prev;
	struct s_node	*next;
}					t_node;

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
	int				hd_dir;
	char			*command_path;
	char			**paths;
	char			***my_envp;
	struct s_node	*command_node;
	struct s_node	*heredoc_node;
}					t_pipes;

//0. parsing
//init and input validation
char	**copy_envp(char **envp);
void	init_parser(t_data *parser, char ***envp);
int		is_unclosed_quote(char *input);
char	*add_spaces(char *input, t_data *parser);
void	update_quote(char c, int *quote);
int		is_missing_pre_space(char *input, int i, int quote);
int		is_missing_post_after_pre_space(char *input, int i);
int		is_missing_post_space(char *input, int i, int quote);
char	*check_pipes(char *line, t_data *parser, int i, int *status);
void	end_pipe_ctrld(char *temp, t_data *parser, char *line, int backup_fd);
int		end_pipe_sigint(char *temp, char *line, int *status, int backup_fd);
void	end_pipe_helper(char **line, char *temp, char **new_line);
int		is_only_pipes(char *input);
void	init_sections(t_data *parser, char *line);
void	init_tokens(t_data *parser);

//lexing
int		lexer(t_data *parser);
int		make_all_redir_nodes(t_data *parser, int i);
t_node	*init_new_node(t_data *parser, t_node *new_node);
int		set_cmd_node(t_data *parser, t_index *index, t_node *new_node);
int		count_args(t_data *parser, int i, int j);
char	**split_to_sections(char const *s, char c, int *error);
int		sectionsplit_strings(char const *s, char c, int i);
char	**split_to_tokens(char const *s, int *error);
int		tokensplit_strings(char const *s, int i);
int		split_checkquote(char const *s, int i, char quote);
void	split_setstring(char const *s, int *i, int *len, int *quote);
char	**split_freearray(char **array, int j, int *error);

//parsing
void	handle_cmd(t_node *tmp, t_data *parser, int status);
char	**handle_cmd_helper(char **cmd, t_data *parser, int status, int arg);
void	init_new_cmd(char **cmd, t_data *parser, t_exp *expand);
void	init_new_arg(t_exp *expand, int *new_arg);
void	handle_filename(t_node *tmp, t_data *parser, int status);
char	*handle_filename_helper(char *file, t_data *parser, int status);
char	*handle_check_quotes(char *s, t_data *parser, t_exp *expand, int arg);
char	*handle_quotes(char *s, t_data *parser, t_exp *expand);
char	*find_envp(t_exp *expand, int i);
void	init_exp(t_exp *exp, int status, t_data *parser, t_pipes *my_pipes);
char	*find_exp(char *arg, int *i, int *k, t_exp *expand);
char	*find_replacer(char *arg, int i, t_exp *expand);
void	append_char(char **new_string, char c, t_exp *expand);
void	append_char_heredoc(char **new_string, char c,
			t_pipes *my_pipes, t_node *heredoc_node);
void	append_replacer(char **new_string, char *replacer, int is_freeable,
			t_exp *expand);
int		expand_line_helper(char *line, char **new_line, t_exp *expand, int i);
int		is_redirection(char *token);
void	handle_quotes_in_expansion(t_exp *expand, int *new_arg, int *arg);
void	count_expandable(char *arg, int *i, int *j);
void	update_single_quote(char c, int *quote, int *d_quote);

//heredoc
int		heredoc(t_node *curr, t_pipes *my_pipes, int status);
void	heredoc_mkdir(char **envp, t_pipes *my_pipes, int status);
void	heredoc_rmdir(char **envp, t_pipes *my_pipes);
void	handle_tmpfile(t_pipes *my_pipes);
void	check_tmp_dir(t_pipes *my_pipes);
void	check_rmdir_success(t_pipes *my_pipes, pid_t pid);
int		hd_expand_line_helper(char *line, char **new_line,
			t_exp *expand, int i);

//1. execution
//execution handler
int		begin_execution(t_node *list, char ***envp, int status);

//close, free and reset
void	close_pipeline_fds(t_pipes *my_pipes);
void	reset_properties(t_pipes *my_pipes);
void	free_my_pipes(t_pipes *my_pipes);
void	close_all_fds(t_pipes *my_pipes);
void	cleanup_in_exec(t_pipes *my_pipes, t_node *list);

//redirections
void	handle_redirections(t_pipes *my_pipes);
void	open_infile(char *file, t_pipes *my_pipes);
void	set_outfile(char *file, enum s_type redir_type, t_pipes *my_pipes);

//builtins
int		execute_builtin(t_node *node, t_pipes *my_pipes);
void	execute_echo(t_node *node);
void	execute_env(char ***envp);
void	execute_pwd(t_pipes *my_pipes, char ***envp, int i, t_exp *expand);
void	execute_export(char **cmd, char ***envp, t_pipes *my_pipes);
void	execute_cd(char **cmd, t_pipes *my_pipes);
void	execute_unset(char **cmd, char ***envp, t_pipes *my_pipes);
void	execute_exit(char **cmd, t_pipes *my_pipes);
char	*add_quotes_export(char **envp, int i);
int		is_replacer_envp(char ***envp, char *arg);
int		export_fill_envp(char ***new_envp, char **cmd, char **envp,
			t_pipes *my_pipes);
int		add_exported_envp(char ***new_envp, char **cmd, int i,
			t_pipes *my_pipes);
char	**fill_unset_envp(char ***new_envp, char **cmd,
			char **envp, t_pipes *my_pipes);
int		export_validation(char **cmd, int i);
int		is_valid_to_export(char *arg);
int		count_args_to_export(char **cmd);
int		find_existing_envp(char ***new_envp, char **cmd, char **envp, int i);
int		find_unset_element(char *arg, char **envp);
int		find_first_unset_element(char **cmd, char **envp, int j);
int		find_next_unset_element(int *i, int *j, char **cmd, char **envp);
void	fatal_sort_for_export_error(char **export, int elements,
			t_pipes *my_pipes);
void	fatal_export_unset_error(char **new_envp, t_pipes *my_pipes);
void	fatal_pwd_error(char *msg, t_pipes *my_pipes, int i, t_exp *expand);

//execution external
int		execute_executable(t_node *node, t_pipes *my_pipes);

//execution utils
char	**get_paths(t_pipes *my_pipes);
char	*get_absolute_path(char **paths, char *command, t_pipes *my_pipes);
int		is_builtin(char *command);
int		get_pipe_amount(t_node *list);

//2. other
//cleanup
void	free_array(char **array);
void	free_nodes(t_node *node);
void	free_sections_tokens(t_data *parser);
void	free_expand(t_exp *expand);
void	fatal_parsing_error(t_data *parser, t_exp *expand,
			char *input, char *msg);
void	fatal_exec_error(char *msg, t_pipes *my_pipes,
			t_node *list, char *conversion);

//utils
int		count_elements(char **tokens);
int		is_quote(char *s);
int		is_only_quotes(char *s);
int		is_exp_delimiter(char c);
int		is_char_redirection(char c);
int		is_whitespace(char c);
void	print_error(char *msg, char *conversion_1, char *conversion_2);

//signals
void	init_signal_handler(int sig);
void	heredoc_signal(int sig);
void	parent_signal(int sig);
void	listen_to_signals(int in_parent);

#endif
