/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:57:47 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/09 10:58:11 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	do_echo(t_ms **ms)
{
	t_cmd	*temp;
	int	n_flag;
	int	i;

	(*ms)->exit_code = 1;
	i = 1;
	n_flag = 0;
	temp = (*ms)->cmd;
	if (!temp->args[1])
		printf("\n");
	else
		echo_w_args(ms, temp, n_flag, i);
	(*ms)->exit_code = 0;
}

void	echo_w_args(t_ms **ms, t_cmd *temp, int n_flag, int i)
{
	if (!(ft_strncmp(temp->args[1], "-n", 2)))
		n_flag = 1;
	if (n_flag == 1)
		echo_n_flag(ms, temp);
	else if (n_flag == 0)
	{
		while (temp->args[i])
		{
			echo_writter(ms, temp->args[i]);
			if (temp->args[i+1])
				printf(" ");
			i++;
		}
	}
}

void	echo_n_flag(t_ms **ms, t_cmd *temp)
{
	int	i;
	if (!temp->args[2])
		;
	else
	{
		i = 2;
		while (temp->args[i])
		{
			echo_writter(ms, temp->args[i]);
			if (temp->args[i+1])
				printf(" ");
			i++;
		}
		printf("\n");
	}
}

void	echo_writter(t_ms **ms, char *string)
{
	t_env	*temp;
	char	*after_dollar;

	temp = (*ms)->env_s;
	if (!(ft_strncmp(string, "$", 1)))
	{
		after_dollar = ft_strdup(string+1);
		while (temp)
		{
			if (!ft_strncmp(after_dollar, temp->env_name, ft_strlen(after_dollar)))
				printf("%s",temp->env_value);
			temp = temp->next;
		}
	}
	else
		printf("%s", string);
}
