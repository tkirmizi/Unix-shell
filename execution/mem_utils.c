/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:51:45 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/18 11:25:21 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_cmd(t_cmd **cmd)
{
	t_cmd	*temp;

	temp = (*cmd);
	while (temp)
	{
		if (temp->path_for_excat)
			free(temp->path_for_excat);
		temp = temp->next;
	}
}

void	free_commands(t_cmd *cmd)
{
	t_cmd	*next;
	int		i;

	i = 0;
	if (!cmd)
		return ;
	while (cmd)
	{
		next = cmd->next;
		if (cmd->args)
		{
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		cmd->args = NULL;
		free(cmd);
		cmd = next;
	}
}

void	data_alloc(t_pipe_data *data, int cmd_count)
{
	t_pipe_data	*temp;

	temp = data;
	temp->cmd_count = cmd_count;
	temp->pids = (pid_t *)malloc(cmd_count * sizeof(pid_t));
	temp->cmd_index = 0;
}
