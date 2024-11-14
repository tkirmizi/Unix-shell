/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:28:50 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 21:12:25 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdbool.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <termios.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>

# define MAX_PATH_LENGTH 1024

extern int		g_signal;
typedef bool	t_bool;
typedef struct s_signals
{
	int	sigint;
	int	sigquit;
}	t_signals;
typedef struct s_env_var
{
	char	*name;
	char	*value;
	size_t	name_len;
	size_t	value_len;
}	t_env_var;

typedef struct s_pipe_data
{
	int		(*fds)[2];
	pid_t	*pids;
	int		cmd_count;
	int		cmd_index;
}	t_pipe_data;

typedef enum s_type
{
	WORD,
	PIPE,
	IN,
	H_DOC,
	OUT,
	APPEND,
	ERR,
	END
}	t_type;

typedef struct s_expansion
{
	char	*result;
	size_t	pos;
	size_t	len;
	int		in_squote;
	int		in_dquote;
	int		last_exit_code;
	pid_t	shell_pid;
}	t_expansion;

typedef struct s_token
{
	t_type			type;
	struct s_token	*prev;
	struct s_token	*next;
	char			*value;
}	t_token;

typedef struct s_cmd
{
	t_token			*token;
	struct s_cmd	*prev;
	struct s_cmd	*next;
	int				fd_in;
	int				fd_out;
	char			**args;
	char			**path_for_excat;
	char			*builtin[6];
}	t_cmd;

typedef struct s_env
{
	char			*env_name;
	char			*env_value;
	struct s_env	*next;
}	t_env;

typedef struct s_ms
{
	t_token				*token;
	t_cmd				*cmd;
	t_env				*env_s;
	pid_t				*pids;
	int					exit_code;
	char				*input;
	char				**env;
	char				**all_cmd_paths;
	int					pos;
	struct s_signals	*sig;
}	t_ms;

typedef struct s_lex
{
	char	*input;
	int		pos;
	int		sq;
	int		dq;
}	t_lex;

typedef struct s_ft
{
	char	*name;
	void	(*ft)(char **argv, t_env **env_s, t_ms *ms);
}	t_ft;

typedef struct s_expand {
	char	*expanded_str;
	char	*str;
	int		*s_flag;
	t_ms	*ms;
	int		len;
	int		*i;
}	t_expand;

typedef struct s_split
{
	int		split_count;
	t_cmd	*cmd;
}	t_split;

t_env		*ft_save_env(char **env, t_env *env_struct);
t_env		*find_last(t_env *env_s);
t_env_var	*create_env_var(const char *name, const char *value);
t_token		*ft_create_pipe_t(t_lex *lexer);
t_token		*ft_input_token(t_lex *lexer);
t_token		*ft_output_token(t_lex *lexer);
t_token		*ft_word_token(t_lex *lex);
t_token		*ft_endof_t(void);
t_token		*ft_ext_word_val(int start, t_lex *lex);
t_token		*open_quote_val(void);
t_token		*cr_empty_q_t(void);
t_expansion	*init_expansion(void);
t_lex		*lexer_init(t_ms *ms);
t_token		*get_token(t_lex *lexer);

void		destroy_env_var(t_env_var *var);
void		print_commands(t_cmd *cmd);
void		destroy_expansion(t_expansion *exp);
void		display_tokens(t_token *token);
void		free_commands(t_cmd *cmd);
void		free_tokens(t_token *token);
void		ext_quote_val(t_lex *lex, char quote);
void		subtitute_free_args(char ***args, int *n, int *s_flag);
void		ft_strnjoin_helper(char *dest, const char *src, size_t n);
void		ft_eval_args(t_cmd *cmd, t_ms *ms);
void		heredoc_all(int fd, char *del, t_ms *mini);
void		ft_del_array(char ***arr_ptr, int index);
void		remove_cmd_node(t_ms *mini, t_cmd *node_to_remove);
void		ft_set_env(char *name, char *value, int overwrite, t_env *env_s);
void		ft_set_node(t_env **env_s, char *token, char *type);
void		free_struct(t_env **stack);
void		ft_set_env(char *name, char *value, int overwrite, t_env *env_s);
void		signal_handler(void);
void		ft_itoa_buf(int n, char *buf);
void		ft_strnjoin_helper(char *dest, const char *src, size_t n);
void		subtitute_free_args(char ***args, int *n, int *s_flag);
void		exec_for_multi(t_ms **ms, t_cmd **cmd);
void		find_exact_path(t_ms **ms, t_cmd **cmd, int *i);
void		arg_join(t_cmd **cmd);
void		execution(t_ms *ms);
void		ft_set_builtin(t_cmd *cmd);
void		do_env(t_ms **ms);
void		do_pwd(t_ms **ms);
void		do_builtin(t_ms **ms, t_cmd **cmd);
void		do_export(t_ms **ms);
void		check_export_exist(t_ms **ms, char *string);
void		export_itself(t_ms **ms, char *string);
void		update_path(t_ms **ms);
void		do_unset(t_ms **ms);
void		unset_itself(t_ms **ms, char *string);
void		unset_else(t_ms **ms, t_env *temp, t_env *temp2, char *string);
void		do_cd(t_ms **ms);
void		change_pwd_oldpwd(t_ms **ms);
void		chdir_getcwd_all(t_ms **ms, char *new_pwd);
void		do_echo(t_ms **ms);
void		do_exit(t_ms **ms);
void		ft_write_to_fd(int fd, char *string);
void		multi_exec_cont(t_ms **ms, t_cmd *cmd, t_pipe_data *data);
void		cl_fds_middle(int (*fds)[2], int c_command, int i);
void		cl_fds_last(int (*fds)[2], int c_command);
void		cl_fds_first(int (*fds)[2], int c_command);
void		all_path_joiner(t_ms **ms, t_cmd **cmd);
void		update_shell_lvl(t_ms **ms);
void		one_exec(t_ms **ms, t_cmd **cmd);
void		free_cmd(t_cmd **cmd);
void		multi_exec(t_ms **ms, int cmd_count);
void		reset_expansion(t_expansion *exp);
void		clean_str_array(char **arr, int limit);
void		display_cmd_content(t_cmd *current);
void		redir_for_builtin(t_ms **ms);
void		cleanup_env(t_env *env_s);
void	handle_double_q_cont(size_t *i, t_expansion *exp);
void		handle_split_cont(t_split **split_data, char **split, int *i,
				int *original_count);
void		handle_explanded_sec(t_cmd **cmd, char **expanded, int *i, t_expansion **exp);
void		echo_cont(t_cmd *temp, int i, t_ms *ms);
void 		remove_args(t_cmd *cmd);
void		set_fd_helper(char *re, char *path, t_cmd *cmd);
void		one_exec_cont_sec(t_ms **ms, t_cmd **cmd, t_cmd **temp, int	*i);
void		exec_for_else(t_ms *ms, int status);
void		init_pipes(t_pipe_data *data);
void		close_pipe_fds(t_pipe_data *data);
void		wait_for_children(t_ms **ms, t_pipe_data *data);
void		handle_first_cmd(t_cmd *cmd, t_pipe_data *data);
void		handle_last_cmd(t_cmd *cmd, t_pipe_data *data);
void		handle_middle_cmd(t_cmd *cmd, t_pipe_data *data);
void		fill_array(t_ms **ms, t_env *temp, int i);
void		ft_and_exit(void);
void		process_quote_handler(t_expand *exp, int *i);
int			process_var_expasion_second(t_expand *exp);
int			handle_quote(char *str, t_expansion *exp, char quote_type);
int			count_words(char *str);
int			copy_split_words(char **split, char **args, int i);
int			handle_expanded_arg(char *expanded, t_cmd *cmd, int i, t_expansion *exp);
int			handle_split_args(char **split, t_cmd *cmd, int i, int org_count);
int			handle_quote_cont(char *str, t_expansion *exp, char quote_type, size_t start);
int			count_cmd_args(t_token *tok);
int			store_args(t_cmd *cmd, t_token **tok, int size);
int			lexing(t_ms *ms);
int			extr_args(t_token **token, t_cmd *cmd);
int			parse(t_ms *ms);
int			syntax_error(t_ms *ms);
int			commands(t_ms *ms);
int			not_empty(const char *input);
int			extr_args(t_token **token, t_cmd *cmd);
int			expand(t_ms *ms);
int			handle_split(t_cmd *cmd, char **split, int i, int original_count);
int			handle_expanded(t_cmd *cmd, char *expanded, int i, t_expansion *exp);
int			ft_valid_expand(char **s, t_ms *ms, int *s_flag);
int			process_pid_exit(const char *str, int *i, char **ex_str, t_ms *ms);
int			process_dbl_quotes(char *str, int *i, char **ex_str, t_ms *ms);
int			process_single_qutoes(char *str, int *i, char **ex_str);
int			process_var_expasion_first(char *str, int *i, char **ex_str, t_ms *ms);
int			ft_exp_concan(char *str, int *i, char **ex_str, t_ms *ms);
int			ft_manage_redir(t_ms *mini);
int			ft_valid_file(char *path);
int			set_fd(char *re, char *path, t_cmd *cmd, t_ms *mini);
int			file_exist(t_ms *mini, char *path);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_isspace(int c);
int			process_dbl_quotes(char *str, int *i, char **ex_str, t_ms *ms);
int			process_single_qutoes(char *str, int *i, char **ex_str);
int			ft_valid_expand(char **s, t_ms *ms, int *s_flag);
int			process_single_qutoes(char *str, int *i, char **ex_str);
int			process_dbl_quotes(char *str, int *i, char **ex_str, t_ms *ms);
int			ft_command_counter(t_cmd **command);
int			ft_is_builtin(t_cmd **cmd);
int			ft_is_num(char *string);
int			handle_parse_error(t_ms *ms);
int			expand_args(t_cmd *cmd, t_expansion *exp);
int			init_new_command(t_cmd **cmd);
int			setup_new_cmd(t_cmd **prev, t_token **token, t_cmd **cmd);
int			build_cmd_list(t_token *token, t_cmd **cmds);
int			get_num_len(int n);
int			handle_quote(char *str, t_expansion *exp, char quote_type);
int			handle_quote_cont(char *str, t_expansion *exp, char quote_type, size_t start);
int			fill_word(char **words, char *str, int *i, int word_count);
int			is_valid_n_flag(char *arg);
int			pipe_error_check(t_token *token);
int			redir_error_check(t_token *token);
int			wrong_input(char *input);
char		*ft_strnjoin(char *s1, const char *s2, size_t n);
char		*ft_fetch_env(const char *name, char **env);
char		**ft_generate_args(char **n_as, char **as, char **s_as, const int *n);
char		*ft_strnjoin(char *s1, const char *s2, size_t n);
char		*ft_strncpy(char *dst, const char *src, int len);
char		*find_prev_path(t_ms **ms);
char		*find_last_part(t_ms **ms);
char		*cur_pwd_return(t_ms **ms);
char		*copy_token_value(t_token *token);
char		**ft_generate_args(char **new_args, char **args, char **splt_args,
				const int *n);
char		*handle_single_quotes(char *str, size_t *i, char *result, t_expansion *exp);
char		**ft_realloc_array(char **ptr, size_t new_size);
char		**ft_realloc_array(char **ptr, size_t new_size);
char		**split_expanded(char *str);
char		*expand_variables(char *str, t_expansion *exp);
char		**clean_split_fail(char **words, int word_count);
char		**split_expanded_cont(char *str, char **words);
char		*handle_exit_code(t_expansion *exp, char **result);
char		*handle_pid(t_expansion *exp, char **result);
char		*handle_env_var(char *str, size_t *i, char **result);
char		*handle_dollar(char *str, size_t *i, t_expansion *exp, char **result);
char		*handle_double_quotes(char *str, size_t *i, char *result, t_expansion *exp);
char		*handle_regular_char(char *str, size_t *i, char *result);
char		**allocate_new_args(char **args, const char *temp, char **split_args,
						 int *n);
char		*str_expander(t_expand *exp);

#endif
