NAME = minishell

CC = cc

CFLAGS = -lreadline -L /opt/homebrew/opt/readline/lib  -I /opt/homebrew/opt/readline/include -L./libft -lft  

LIBFT = libft/libft.a
LIBFT_DIR = libft

SRCS = 	parsing/signal.c parsing/arg_process.c parsing/command_display.c parsing/command_struct.c parsing/env_var.c \
		parsing/error.c parsing/exp_handle_cont.c parsing/exp_handle.c parsing/heredoc.c parsing/lex_init.c \
		parsing/mem_manage.c parsing/node_manage.c parsing/other_utils.c parsing/parse.c parsing/process_var.c \
		parsing/quote_handle.c parsing/quote.c parsing/redir.c parsing/split_handle.c parsing/split.c \
		parsing/token_manage.c parsing/token_type.c parsing/utils.c execution/builtin_core.c execution/cd_pwd_utils.c \
		execution/cd_pwd.c execution/echo.c execution/env_handle.c execution/exec_cont.c execution/execution.c \
		execution/export.c execution/fds_handle.c execution/main.c execution/mem_utils.c execution/other_bin.c \
		execution/path_man.c execution/pipe_handle.c execution/some_utils.c execution/unset_bin.c parsing/other.c \
		parsing/del_node.c \

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
