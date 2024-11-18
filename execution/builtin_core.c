/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_core.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:43:39 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/15 11:37:44 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_set_builtin(t_cmd *cmd)
{
	cmd->builtin[0] = "cd";
	cmd->builtin[1] = "pwd";
	cmd->builtin[2] = "echo";
	cmd->builtin[3] = "env";
	cmd->builtin[4] = "export";
	cmd->builtin[5] = "unset";
}

int	ft_is_builtin(t_cmd **cmd)
{
	int		i;
	t_cmd	*temp;

	i = 0;
	temp = (*cmd);
	ft_set_builtin(*cmd);
	while (i < 6)
	{
		if (!(ft_strncmp(temp->args[0], temp->builtin[i],
					ft_strlen(temp->args[0]))))
			return (i);
		i++;
	}
	return (10);
}

void	do_builtin(t_ms **ms, t_cmd **cmd)
{
	int	i;

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
}

void	redir_for_builtin(t_ms **ms)
{
	t_cmd	*temp;

	temp = (*ms)->cmd;
	if (temp->fd_out != STDOUT_FILENO)
		dup2(temp->fd_out, STDOUT_FILENO);
	if (temp->fd_in != STDIN_FILENO)
		dup2(temp->fd_in, STDIN_FILENO);
}
