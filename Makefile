NAME = minishell

CC = gcc -g
#CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L./libft -lft -lreadline

LIBFT = libft/libft.a
LIBFT_DIR = libft

SRCS = minishell.c \
		signal_handler.c \
		lex.c \

OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

$(LIBFT):
	@echo "Making Libft..."
	make -sC $(LIBFT_DIR)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(OBJS) $(LIBFT_DIR)/*.o
	@make -sC $(LIBFT_DIR) clean
	
fclean: clean
	rm -f $(NAME)
	@make -sC $(LIBFT_DIR) fclean

re: fclean all

ac: all clean

.PHONY: all clean fclean re
