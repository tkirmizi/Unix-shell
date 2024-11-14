/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_bin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:49:30 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 19:23:15 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	do_env(t_ms **ms)
{
	int		i;
	t_ms	*temp;

	(*ms)->exit_code = 1;
	i = -1;
	temp = (*ms);
	redir_for_builtin(ms);
	while (temp->env[++i])
	{
		write(STDOUT_FILENO, temp->env[i], ft_strlen(temp->env[i]));
		write(STDOUT_FILENO, "\n", 1);
	}
	(*ms)->exit_code = 0;
}

void	do_exit(t_ms **ms)
{
	(*ms)->exit_code = 1;
	ft_write_to_fd(STDERR_FILENO, "exit");
	if ((*ms)->cmd->args[1] && (*ms)->cmd->args[2])
		ft_write_to_fd(STDERR_FILENO, "too many arguments");
	else if ((*ms)->cmd->args[1])
	{
		if (!(ft_is_num((*ms)->cmd->args[1])))
		{
			(*ms)->exit_code = 255;
			ft_write_to_fd(STDERR_FILENO, "numeric argument required");
		}
		else
			(*ms)->exit_code = ft_atoi((*ms)->cmd->args[1]);
	}
	else
		(*ms)->exit_code = 0;
	exit((*ms)->exit_code);
	free_commands((*ms)->cmd);
	cleanup_env((*ms)->env_s);
	clear_history();
	rl_clear_history();
}
