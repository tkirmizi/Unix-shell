/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:56:37 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/09 10:56:55 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	do_unset(t_ms **ms)
{
	t_env *temp;
	int	i;

	(*ms)->exit_code = 1;
	i = 1;
	temp = (*ms)->env_s;
	while ((*ms)->cmd->args[i] != NULL)
	{
		unset_itself(ms, (*ms)->cmd->args[i]);
		i++;
	}
	(*ms)->exit_code = 0;
}
void	unset_itself(t_ms **ms, char *string)
{
	t_env *temp;
	t_env *temp2;
	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(string, temp->env_name, ft_strlen(string))))
			break;
		temp2 = temp;
		temp = temp->next;
	}
	if (!temp)
		printf("There is no variable such as '%s'\n", string);
	else if (temp->next == NULL)
	{
		temp2->next = NULL;
		free(temp);
		update_path(ms);
	}
	else
	{
		unset_else(ms, temp, temp2, string);
		update_path(ms);
	}
}
void	unset_else(t_ms **ms, t_env *temp, t_env *temp2, char *string)
{
	if (!(ft_strncmp(temp->env_name, (*ms)->env_s->env_name, ft_strlen(temp->env_name))))
	{
		(*ms)->env_s = (*ms)->env_s->next;
		temp->next = NULL;
		free(temp);
	}
	else
	{
		temp2->next = temp->next;
		temp->next = NULL;
		temp->env_name = NULL;
		temp->env_value = NULL;
		free(temp);
	}
}
