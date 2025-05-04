NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a

HEADERS = minishell.h $(LIBFTDIR)/libft.h
SRC = add_spaces.c add_spaces_utils.c add_spaces_utils_2.c \
	builtin_cd.c builtin_echo.c builtin_env.c builtin_exit.c \
	builtin_export_unset_errors.c builtin_export_utils.c \
	builtin_export.c builtin_pwd.c builtin_unset.c \
	check_pipes.c check_pipes_utils.c cleanup.c \
	copy_envp.c execution_builtin.c execution_close_and_reset.c \
	execution_external.c execution_handler.c execution_redirections.c \
	execution_utils.c expand_append.c expand_cmd.c expand_line.c \
	expand_utils.c expand_utils_2.c expand.c heredoc.c heredoc_tmpfile.c \
	heredoc_tmpfile_utils.c init.c lexing_cmd.c lexing_redir.c lexing.c \
	main.c signals.c split_to_sections.c split_to_sections_utils.c \
	split_to_tokens.c split_to_tokens_utils.c utils.c utils_2.c
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
