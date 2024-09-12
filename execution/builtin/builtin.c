/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:54:58 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/12 18:06:56 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	ft_set_builtin(t_cmd *cmd)
{
	cmd->builtin[0] = "cd";
	cmd->builtin[1] = "pwd";
	cmd->builtin[2] = "echo";
	cmd->builtin[3] = "env";
	cmd->builtin[4] = "export";
	cmd->builtin[5] = "unset";
	cmd->builtin[6] = "exit";
}

int ft_is_builtin(t_cmd **cmd)
{
	int i;
	t_cmd *temp;

	i = 0;
	temp = (*cmd);
	ft_set_builtin(*cmd);
	
	while (i < 7)
	{
		if (!(ft_strncmp((*cmd)->args[0], (*cmd)->builtin[i], ft_strlen((*cmd)->args[0]))))
		{
			return (i);
		}
		i++;
	}
	return (10);
}

void	do_builtin(t_ms **ms, t_cmd **cmd)
{
	int i;
	i = ft_is_builtin(cmd);
	if (i == 0)
		do_cd(ms);
	if (i == 1)
		do_pwd(ms);
	if (i == 2)
		do_echo(ms);
	if (i == 3)
		do_env(ms);
	if (i == 4)
		do_export(ms);
	if (i == 5)
		do_unset(ms);
	if (i == 6)
		do_exit(ms);
}