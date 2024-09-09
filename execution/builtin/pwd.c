/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:55:24 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/09 10:55:38 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	do_pwd(t_ms **ms)
{
	t_env *temp;

	(*ms)->exit_code = 1;
	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "PWD", 3)))
			break;
		temp = temp->next;
	}
	printf("%s\n", temp->env_value);
	(*ms)->exit_code = 0;
}