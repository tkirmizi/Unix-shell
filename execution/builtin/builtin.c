/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:54:58 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/09 10:55:15 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	ft_set_builtin(t_ms *ms)
{
	ms->bin.builtin[0] = "cd";
	ms->bin.builtin[1] = "pwd";
	ms->bin.builtin[2] = "echo";
	ms->bin.builtin[3] = "env";
	ms->bin.builtin[4] = "export";
	ms->bin.builtin[5] = "unset";
	ms->bin.builtin[6] = "exit";
}

int ft_is_builtin(t_ms *ms)
{
	int i;

	i = 0;
	ft_set_builtin(ms);
	
	while (i < 7)
	{
		if (!(ft_strncmp(ms->cmd->args[0], ms->bin.builtin[i], sizeof(ms->cmd->args[0]))))
			return (i); // builtin demek yani
		i++;
	}
	return (0);
}

void	do_builtin(t_ms *ms)
{
	int i = ft_is_builtin(ms);
	if (i == 0)
		do_cd(&ms);
	if (i == 1)
		do_pwd(&ms);
	if (i == 2)
		do_echo(&ms);
	if (i == 3)
		do_env(&ms);
	if (i == 4)
		do_export(&ms);
	if (i == 5)
		do_unset(&ms);
	if (i == 6)
		do_exit(&ms);
}