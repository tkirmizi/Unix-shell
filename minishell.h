typedef struct s_mini {
  t_token *token;
  struct s_mini *prev;
  struct s_mini *next;
  int fd_in;
  int fd_out;
  char **args;
} t_mini;
