/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:43:15 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 12:43:30 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void heredoc_all(int fd, char *del, t_ms *mini)
{
	char *line;

	while (1)
	{
		line = readline(">");
		if (!ft_strcmp(line, del))
		{
			free(line);
			break;
		}
		if (ft_strchr(line, '$'))
		{
			ft_valid_expand(&line, mini, 0);
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

void set_fd_helper(char *re, char *path, t_cmd *cmd)
{
	if ((!ft_strcmp(re, ">>") || !ft_strcmp(re, ">")) &&
		cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	if (!ft_strcmp(re, ">"))
	{
		cmd->fd_out = open(path, O_CREAT | O_RDWR | O_APPEND, 0644);
	}
	if (!ft_strcmp(re, ">>"))
	{
		cmd->fd_out = open(path, O_CREAT | O_RDWR | O_APPEND, 0644);
	}
	if ((!ft_strcmp(re, "<<") || !ft_strcmp(re, "<")) &&
		cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (!ft_strcmp(re, "<"))
		cmd->fd_in = open(path, O_RDONLY);
}

