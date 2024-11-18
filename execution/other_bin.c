/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_bin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:49:30 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/18 11:29:55 by tkirmizi         ###   ########.fr       */
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

void	exit_w_out_free(char *str, int i)
{
	perror(str);
	exit(i);
}

void	free_double_ptr(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

void	free_env_array(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
		free(env[i++]);
	free(env);
}

void	do_exit(t_ms **ms)
{
	if (!ms || !*ms)
		exit(1);
	(*ms)->exit_code = 1;
	ft_write_to_fd(STDERR_FILENO, "exit");
	if ((*ms)->cmd && (*ms)->cmd->args)
	{
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
	}
	clear_history();
	rl_clear_history();
	exit((*ms)->exit_code);
}
