NAME = minishell
CC = cc -g
CFLAGS = -Wall -Wextra -Werror
LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a

HEADERS = minishell.h $(LIBFTDIR)/libft.h
SRC = add_spaces.c add_spaces_utils.c add_spaces_utils_2.c \
	builtins.c builtins_utils.c builtins_envp.c \
	builtins_envp_utils.c builtins_envp_utils_2.c \
	builtins_envp_utils_3.c builtins_envp_cleanup.c \
	check_pipes.c check_pipes_utils.c cleanup.c \
	copy_envp.c expand_cmd.c expand_line.c expand_utils.c \
	expand_utils_2.c expand.c expand_append.c \
	split_to_sections.c split_to_sections_utils.c execution_handler.c \
	execution_redirections.c execution_external.c \
	execution_close_and_reset.c execution_builtin.c \
	execution_utils.c heredoc.c heredoc_tmpfile.c init.c \
	lexing_cmd.c lexing_redir.c lexing.c main.c signals.c \
	utils.c heredoc_tmpfile_utils.c \
	utils_2.c split_to_tokens.c split_to_tokens_utils.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(LIBFT):
	@make all -C $(LIBFTDIR)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(OBJ) $(LIBFT) -o $(NAME) -lreadline

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -I$(LIBFTDIR) -o $@ -c $<

clean:
	@rm -rf $(OBJ)
	@make clean -C $(LIBFTDIR)

fclean: clean
	@rm -rf $(NAME) $(LIBFT)

re: clean all

.PHONY: all, clean, fclean, re
