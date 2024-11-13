/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:51:45 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 13:53:06 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_cmd(t_cmd **cmd)
{
	t_cmd *temp;

	temp = (*cmd);
	while (temp)
	{
		if (temp->path_for_excat)
			free(temp->path_for_excat);
		temp = temp->next;
	}
}

void free_commands(t_cmd *cmd)
{
	t_cmd *tmp;
	int i;

	tmp = NULL;
	while (cmd)
	{
		tmp = cmd->next;
		i = 0;
		while (cmd->args && cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
		free(cmd);
		cmd = tmp;
	}
}


