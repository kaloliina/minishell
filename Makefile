NAME = minishell
CC = cc -g
CFLAGS =
LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a

HEADERS = minishell.h $(LIBFTDIR)/libft.h
SRC = main.c add_spaces.c cleanup.c ft_ms_split.c \
	ft_ms_split_utils.c utils.c signals.c init.c \
	lexing.c lexing_utils.c get_paths.c \
	handle_files.c test.c builtins.c expansions.c \
	heredoc.c builtins_utils.c expansions_utils.c \
	builtins_envp.c add_spaces_utils.c check_pipes.c \
	copy_envp.c
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
