/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 18:30:22 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/08/28 17:08:51 by tkirmizi         ###   ########.fr       */
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
typedef struct s_command
{
	// t_token *token; lexer/parser partner will take care of it
	struct s_mini *prev;
	struct s_mini *next;
	int fd_in;
	int fd_out;
	char **args; 
} t_command;

#endif