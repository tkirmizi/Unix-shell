/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:41:08 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 19:27:24 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_first_cmd(t_cmd *cmd, t_pipe_data *data)
{
	cl_fds_first(data->fds, data->cmd_count);
	dup2(data->fds[0][1], STDOUT_FILENO);
	dup2(cmd->fd_out, data->fds[0][1]);
	close(data->fds[0][1]);
}

void	handle_last_cmd(t_cmd *cmd, t_pipe_data *data)
{
	cl_fds_last(data->fds, data->cmd_count);
	dup2(data->fds[data->cmd_count - 2][0], STDIN_FILENO);
	dup2(cmd->fd_in, data->fds[data->cmd_count - 2][0]);
	dup2(cmd->fd_out, STDOUT_FILENO);
	close(data->fds[data->cmd_count - 2][0]);
}

void	handle_middle_cmd(t_cmd *cmd, t_pipe_data *data)
{
	cl_fds_middle(data->fds, data->cmd_count, data->cmd_index);
	dup2(data->fds[data->cmd_index - 1][0], STDIN_FILENO);
	dup2(cmd->fd_in, data->fds[data->cmd_index - 1][0]);
	dup2(data->fds[data->cmd_index][1], STDOUT_FILENO);
	dup2(cmd->fd_out, data->fds[data->cmd_index][1]);
	close(data->fds[data->cmd_index - 1][0]);
	close(data->fds[data->cmd_index][1]);
}

void	ft_and_exit(void)
{
	ft_write_to_fd(2, "error: execution");
	exit(127);
}
