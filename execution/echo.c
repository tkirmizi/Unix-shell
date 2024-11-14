/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:44:28 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 19:29:11 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	do_echo(t_ms **ms)
{
	t_cmd	*temp;
	int		n_flag;
	int		i;

	if (!ms || !*ms || !(*ms)->cmd)
		return ;
	temp = (*ms)->cmd;
	i = 1;
	n_flag = 0;
	if (!temp->args || !temp->args[1])
	{
		write(1, "\n", 1);
		return ;
	}
	while (temp->args[i] && is_valid_n_flag(temp->args[i]))
	{
		n_flag = 1;
		i++;
	}
	echo_cont(temp, i, *ms);
	if (!n_flag)
		write(1, "\n", 1);
}

void	echo_cont(t_cmd *temp, int i, t_ms *ms)
{
	int	s_flag;

	while (temp->args[i])
	{
		if (ft_strchr(temp->args[i], '$') && !ft_strchr(temp->args[i], '\''))
		{
			s_flag = 0;
			ft_valid_expand(&(temp->args[i]), ms, &s_flag);
		}
		write(1, temp->args[i], ft_strlen(temp->args[i]));
		if (temp->args[i + 1])
			write(1, " ", 1);
		i++;
	}
}

int	is_valid_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
