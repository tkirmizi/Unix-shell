/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:42:48 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 13:43:08 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void cl_fds_first(int (*fds)[2], int cmd_count)
{
	int i;
	int j;

	i = 0;
	close(fds[0][0]);
	while (i < cmd_count - 1)
	{
		j = 0;
		while (j < 2)
		{
			if (i != 0)
				close(fds[i][j]);
			j++;
		}
		i++;
	}
}

void cl_fds_last(int (*fds)[2], int cmd_count)
{
	int i;
	int j;

	i = 0;
	while (i < cmd_count - 1)
	{
		j = 0;
		while (j < 2)
		{
			if (i != cmd_count - 2)
				close(fds[i][j]);
			j++;
		}
		i++;
	}
	close(fds[cmd_count - 2][1]);
}

void cl_fds_middle(int (*fds)[2], int cmd_count, int index)
{
	int i;

	i = 0;
	while (i < cmd_count - 1)
	{
		if (i != index - 1)
			close(fds[i][0]);
		if (i != index)
			close(fds[i][1]);
		i++;
	}
}