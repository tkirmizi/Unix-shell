/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:29:49 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/08/28 17:08:21 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	
}

int		command_counter(t_command *command)
{
	int	i;
	
	i = 0;
	if (command -> args == NULL)
		return (i);
	else
	{
		if (command -> next == NULL)
			return (1);
		else
		{
			while (command -> next != NULL)
			{
				i++;
				command = command -> next;
			}
			return (i);
		}
	}
}
