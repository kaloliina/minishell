#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <signal.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>




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
	char			*file;
	char			*delimiter;
	char			**cmd;
	struct s_node	*prev;
	struct s_node	*next;
}					t_node;

typedef struct s_ast
{
	int			sections_amount;
	char		**sections;
	char		***tokens;
	t_node		*first;
}				t_ast;

typedef struct t_pipe
{
	int				*pipes;
	char			*command_path;
	char			**paths;
	char			**my_envp;
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
}					t_pipes;

//init
t_node	*init_new_node(t_ast *ast, t_node *new_node);
void	init_sections(t_ast *ast, char *line);
void	init_tokens(t_ast *ast);
void	init_tokens_struct(t_ast *ast);
int		set_sections(t_ast *ast, char **tmp_sections);
void	set_last_section(t_ast *ast, int i, char **tmp_sections);

//lexing
char	*add_spaces(char *input);
int		ft_ms_strings(char const *s, char c, int i);
int		ft_ms_checkquote(char const *s, int i, char quote);
char	**ft_ms_freearray(char **array, int j, int *error);
char	**ft_ms_split(char const *s, char c, int *error);
int		make_node(t_ast *ast, int i, int j, t_node **first);
int		lexer(t_ast *ast);

//parsing
int		is_redirection(char *token);
int		is_char_redirection(char c);
int		is_redirection_char(char *s);
void	make_pipe_node(t_ast *ast, t_node **first);
char	*handle_expandables(char *line, char **envp);
char	*add_replacer(char *line, char *replacer, int k, int j);
char	*find_envp(char *exp, char **envp);

//cleanup
void	free_struct(t_ast *ast);
void	free_sections(t_ast *ast, int i);
void	free_array(char **array);

//utils
int		count_elements(char **tokens);
void	signal_handler(int sig);

//builtins
void	execute_echo(t_node *node, char **envp);
void	execute_env(char **envp);
void	execute_pwd();
void	execute_export(char **cmd, char ***envp);

char **get_paths(char *envp[]);
char *get_absolute_path(char **paths, char *command);
int	open_infile(char *file);
int	set_outfile(char *file, enum s_type redir_type);
char	**loop_nodes(t_node *list, char *envp[]);
void	heredoc(t_node *node, t_pipes *my_pipes, char **envp, char **paths);

#endif
