/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:29:49 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/08/30 13:13:16 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_mini *mini;
	while (1)
	{
		execution(mini->cmd);
	}
}

void	execution(t_mini *mini)
{
	int	count_command; // count
	count_command = command_counter(mini->cmd);
	if (count_command == 1) // one execution
		one_exec(mini);
	// else
	//	execution with pipes.
}

int		command_counter(t_cmd *command)
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

void	one_exec(t_mini *mini)
{
	char  *path = "PATH";
	t_mini *temp = mini;
	
	while (temp->env_p != NULL)
	{
		if (ft_strcmp(temp->env_p->env_name, path) == 0)
		{
			printf("env name = %s\n", temp->env_p->env_name);
			break;
		}
		temp->env_p = temp->env_p->next;
	}
}
