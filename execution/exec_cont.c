/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cont.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:38:25 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 13:58:27 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void multi_exec_cont(t_ms **ms, t_cmd *cmd, t_pipe_data *data)
{
	data->pids[data->cmd_index] = fork();
	if (data->pids[data->cmd_index] == -1)
		exit(EXIT_FAILURE);
	if (data->pids[data->cmd_index] == 0)
	{
		if (data->cmd_index == 0)
			handle_first_cmd(cmd, data);
		else if (data->cmd_index == data->cmd_count - 1)
			handle_last_cmd(cmd, data);
		else
			handle_middle_cmd(cmd, data);
		exec_for_multi(ms, &cmd);
		exit(EXIT_SUCCESS);
	}
}

void init_pipes(t_pipe_data *data)
{
	int i;

	i = 0;
	while (i < data->cmd_count - 1)
	{
		if (pipe(data->fds[i]) == -1)
			exit(EXIT_FAILURE);
		i++;
	}
}

void close_pipe_fds(t_pipe_data *data)
{
	int i;

	i = 0;
	while (i < data->cmd_count - 1)
	{
		close(data->fds[i][0]);
		close(data->fds[i][1]);
		i++;
	}
}

void wait_for_children(t_ms **ms, t_pipe_data *data)
{
	int i;
	int status;

	i = 0;
	while (i < data->cmd_count)
	{
		waitpid(data->pids[i], &status, 0);
		(*ms)->exit_code = WEXITSTATUS(status);
		i++;
	}
}

void multi_exec(t_ms **ms, int cmd_count)
{
	t_pipe_data data;
	t_cmd *cmd;
	int fds[cmd_count - 1][2];

	data.fds = fds;
	data.cmd_count = cmd_count;
	data.pids = (pid_t *)malloc(cmd_count * sizeof(pid_t));
	data.cmd_index = 0;
	if (!data.pids)
		return;
	init_pipes(&data);
	cmd = (*ms)->cmd;
	while (cmd)
	{
		multi_exec_cont(ms, cmd, &data);
		data.cmd_index++;
		cmd = cmd->next;
	}
	close_pipe_fds(&data);
	wait_for_children(ms, &data);
	free(data.pids);
}