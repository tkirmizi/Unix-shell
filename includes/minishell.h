/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 18:30:22 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/09 11:05:30 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "Libft/libft.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <signal.h>
typedef struct s_cmd
{
	// t_token *token; lexer/parser partner will take care of it
	struct s_mini *prev;
	struct s_mini *next;
	int fd_in;
	int fd_out;
	char **args; 
}	t_cmd;

typedef struct s_bin
{
	char	*builtin[7];
}	t_bin;

typedef struct s_mini
{
	t_cmd	*cmd;
	t_env *env_s;
	char	**env;
	char  **all_cmd_paths; // for all cmd paths such as /usr/bin or /usr/local/bin
	// pid_t *pids;
	t_bin bin;
	int	exit_code;
}	t_ms;

typedef struct s_env
{
	char *env_name;
	char *env_value;
	struct s_env *next;
}	t_env;

void	execution(t_ms *ms); // main execution
int		ft_command_counter(t_cmd *command);
void	one_exec(t_ms *ms);
void	arg_join(t_ms *ms);
void	ft_set_builtin(t_ms *ms);
int		ft_is_builtin(t_ms *ms);
void	do_builtin(t_ms *ms);
void	do_cd(t_ms **ms);
void	change_pwd_oldpwd(t_ms **ms);
void	chdir_getcwd_all(t_ms **ms, char *new_pwd);
char	*find_prev_path(t_ms **ms);
char	*find_last_part(t_ms **ms);
void	do_echo(t_ms **ms);
void	echo_w_args(t_ms **ms, t_cmd *temp, int n_flag, int i);
void	echo_n_flag(t_ms **ms, t_cmd *temp);
void	echo_writter(t_ms **ms, char *string);
void	do_env(t_ms **ms);
char	*ft_strncpy(char *dst, const char *src, int len);
void	do_exit(t_ms **ms);
int		ft_is_num(char	*string);
void	ft_write_to_fd_nl(int fd, char *string);
void	ft_write_to_fd(int fd, char *string);
void	do_export(t_ms **ms);
void	export_itself(t_ms **ms, char *string);
void	check_export_exist(t_ms **ms, char *string);
void	update_path(t_ms **ms);
void	do_pwd(t_ms **ms);
void	do_unset(t_ms **ms);
void	unset_itself(t_ms **ms, char *string);
void	unset_else(t_ms **ms, t_env *temp, t_env *temp2, char *string);

#endif