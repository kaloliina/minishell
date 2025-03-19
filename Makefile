NAME = minishell
CC = cc
CFLAGS = -Wextra -Wall -Werror
LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a

HEADERS = minishell.h $(LIBFTDIR)/libft.h
SRC = main.c input_init.c cleanup.c ft_ms_split.c \
	ft_ms_split_utils.c utils.c signals.c init.c \
	lexing.c lexing_utils.c
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
