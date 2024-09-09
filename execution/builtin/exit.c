/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:58:26 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/09 11:01:40 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	do_exit(t_ms **ms)
{
	(*ms)->exit_code = 1;
	ft_write_to_fd_nl(STDERR_FILENO, "exit");
	if ((*ms)->cmd->args[1] && (*ms)->cmd->args[2])
		ft_write_to_fd_nl(STDERR_FILENO, "too many arguments");
	else if ((*ms)->cmd->args[1])
	{
		if (!(ft_is_num((*ms)->cmd->args[1])))
		{
			(*ms)->exit_code = 255;
			ft_write_to_fd_nl(STDERR_FILENO, "numeric argument required");
		}
		else 
			(*ms)->exit_code =  ft_atoi((*ms)->cmd->args[1]);
	}
	else
		(*ms)->exit_code = 0;
	exit((*ms)->exit_code);
}

int	ft_is_num(char	*string)
{
	int	i;

	i = 0;
	while (string[i])
	{
		if (string[i] < 48 || string[i] > 57)
			return (0);
		i++;
	}
	return (1);
}

void	ft_write_to_fd_nl(int fd, char *string)
{
	int	i;

	i = 0;
	while (string[i])
		write(fd, &string[i++], 1);
	write(fd, "\n", 1);
}

void	ft_write_to_fd(int fd, char *string)
{
	int	i;

	i = 0;
	while (string[i])
		write(fd, &string[i++], 1);
}
