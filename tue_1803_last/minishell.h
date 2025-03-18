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
	char			**cmd;
	struct s_node	*prev;
	struct s_node	*next;
}					t_node;

typedef struct s_section
{
	char	*section;
	int		number;
}			t_section;

typedef struct s_ast
{
	int			sections_amount;
	t_section	**sections;
	char		***tokens;
	t_node		*first;
}				t_ast;

//init
t_node	*init_new_node(t_ast *ast, t_node *new_node);
void	init_sections(t_ast *ast, char *line);
void	init_tokens(t_ast *ast);
int		set_sections(t_ast *ast, char **tmp_sections);
void	set_last_section(t_ast *ast, int i, char **tmp_sections);

//lexing
int		ft_ms_strings(char const *s, char c);
int		ft_ms_checkquote(char const *s, int i, char quote);
char	**ft_ms_freearray(char **array, int j, int *error);
char	**ft_ms_split(char const *s, char c, int *error);
int		lexer(t_ast *ast);

//parsing
int		is_redirection(char *token);
int		is_char_redirection(char c);
int		is_redirection_char(char *s);
void	make_pipe_node(t_ast *ast, t_node **first);

//cleanup
void	free_struct(t_ast *ast);
void	free_sections(t_ast *ast, int i);
void	free_array(char **array);

//utils
int		count_elements(char **tokens);
void	signal_handler(int sig);

#endif
