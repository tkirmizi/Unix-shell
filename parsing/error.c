/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:35:25 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 12:36:28 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int redir_error_check(t_token *token)
{
	if (!token->next || token->next->type != WORD)
	{
		ft_putstr_fd("error: wrong token \n", 2);
		ft_putstr_fd(token->value, 2);
		ft_putstr_fd("'\n", 2);
		return (1);
	}
	return (0);
}

int pipe_error_check(t_token *token)
{
	if (token->next == NULL || token->next->type == END || token->prev == NULL ||
		token->prev->type != WORD)
		return (ft_putstr_fd("error: wrong token \n", 2), 1);
	return (0);
}

int ft_valid_file(char *path)
{
	struct stat file_stat;

	if (stat(path, &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(path, 2);
			ft_putstr_fd(": is a directory\n", 2);
			exit(126);
		}
	}
	return (1);
}

void handle_directory(char *path)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": is a directory\n", 2);
}

int file_exist(t_ms *mini, char *path)
{
	struct stat file_stat;

	if (stat(path, &file_stat) == 0)
	{
		if (!S_ISDIR(file_stat.st_mode))
		{
			if (access(path, R_OK) == -1)
			{
				mini->exit_code = 126;
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(path, 2);
				ft_putstr_fd(": Permission denied \n", 2);
				return (1);
			}
		}
		else
		{
			mini->exit_code = 126;
			handle_directory(path);
			return (1);
		}
	}
	return (0);
}