NAME = minishell
CC = cc -g
CFLAGS =
LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a

HEADERS = minishell.h $(LIBFTDIR)/libft.h
SRC = add_spaces.c add_spaces_utils.c builtins_envp.c \
	builtins_envp_utils.c builtins.c check_pipes.c \
	cleanup.c copy_envp.c expand_cmd.c expand_line.c \
	expand_utils.c expand_utils_2.c expand.c \
	expand_append.c ft_ms_split.c ft_ms_split_utils.c \
	get_paths.c handle_files.c heredoc.c init.c \
	lexing_cmd.c lexing_redir.c lexing.c main.c \
	signals.c test.c utils.c
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
