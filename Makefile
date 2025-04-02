NAME = minishell
CC = cc
CFLAGS =
LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a

HEADERS = minishell.h $(LIBFTDIR)/libft.h
SRC = main.c input_init.c cleanup.c ft_ms_split.c \
	ft_ms_split_utils.c utils.c signals.c init.c \
	lexing.c lexing_utils.c get_paths.c \
	handle_files.c test.c builtins.c expandables.c \
	heredoc.c
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
