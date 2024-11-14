/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_display.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:33:34 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 19:19:27 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	display_cmd_content(t_cmd *current)
{
	int	idx;

	idx = 0;
	write(1, "Command Arguments: ", 18);
	if (current->args)
	{
		while (current->args[idx])
		{
			write(1, "argument ->|--:", 14);
			write(1, current->args[idx], ft_strlen(current->args[idx]));
			write(1, ":--|\n", 5);
			idx++;
		}
	}
}

void	print_commands(t_cmd *cmd)
{
	t_cmd	*current;

	if (!cmd)
	{
		write(1, "error: no commands \n", 21);
		return ;
	}
	current = cmd;
	while (current)
		current = current->next;
}

void	ft_itoa_buf(int n, char *buf)
{
	int		len;
	long	num;
	int		i;

	if (!buf)
		return ;
	num = n;
	len = get_num_len(num);
	i = len - 1;
	buf[len] = '\0';
	if (num == 0)
		buf[0] = '0';
	if (num < 0)
	{
		buf[0] = '-';
		num = -num;
	}
	while (num > 0)
	{
		buf[i] = (num % 10) + '0';
		num /= 10;
		i--;
	}
}
