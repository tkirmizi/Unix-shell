NAME = minishell
SRCS = minishell.c execution.c cd.c echo.c env.c export.c pwd.c unset.c
OBJS = $(SRCS:.c=.o)

LIBFT_PATH = ./includes/Libft
LIBFT_OBJS = ./includes/Libft/libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

all = $(NAME)

$(LIBFT_OBJS):
	make -C $(LIBFT_PATH)

$(NAME): $(LIBFT_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(LIBFT_OBJS) $(OBJS) -o $(NAME)
	$(RM) $(OBJS)

%o: $.c
	$(CC) $(CFLAGS) -c $< -o <@

clean:
	$(RM) $(OBJS)
	make clean -C $(LIBFT_PATH)

fclean:
	$(RM) $(NAME)
	$(RM) $(OBJS)
	$(RM) $(LIBFT_OBJS)
re: fclean all