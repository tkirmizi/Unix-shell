/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 18:30:22 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/02 13:45:20 by tkirmizi         ###   ########.fr       */
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
}	t_ms;

typedef struct s_env
{
	char *env_name;
	char *env_value;
	struct s_env *next;
}	t_env;

#endif