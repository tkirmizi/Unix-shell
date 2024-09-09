/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:47:07 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/09 11:02:10 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	execution(t_ms *ms) // main execution
{
	char  *path = "PATH";
	t_ms *temp = ms;
	int	count_command;
	count_command = ft_command_counter(ms->cmd);
	
	if (ft_is_builtin(ms) != 10)
		do_builtin(ms);
	else
	{

		while (temp->env_s != NULL)
		{
			if (ft_strncmp(temp->env_s->env_name, path, 4) == 0)
				break;
			temp->env_s = temp->env_s->next;
		}
		ms->all_cmd_paths = ft_split(temp->env_s->env_value, ':');
		arg_join(ms);
		// if (count_command == -5)
		// 	problem 
		if (count_command == 1) // one execution
			one_exec(ms);
// 		else
// 			multi_exec(ms);
	}
}

int		ft_command_counter(t_cmd *command)
{
	int	i;
	
	i = 0;
	if (command -> args == NULL) // no args and what to do
		return (i);
	else if (command -> next == NULL && command -> args) 
			return (1);
	else
	{
		while (command -> next != NULL)
		{
			i++;
			command = command -> next;
		}
		return (i + 1);
	}
}
