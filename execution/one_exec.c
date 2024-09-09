/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:53:43 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/09 10:54:41 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	one_exec(t_ms *ms)
{
	int	i;
	
	i = 0;
	find_exact_path(ms, &i);
	execve(ms->all_cmd_paths[i], ms->cmd->args, ms->env);
}

void	find_exact_path(t_ms *ms, int	*i)
{
	while (ms->all_cmd_paths[*i])  // while (pipex->path_ptr[*i])
	{
		if (access(ms->all_cmd_paths[*i], X_OK) == 0)
			return ;
		(*i)++;
	}
	// if it comes here means problem on find exact executable, so i have to handle it.
}

void	arg_join(t_ms *ms)
{
	int		i;
	char	*temp;

	i = 0;
	while (ms->all_cmd_paths[i])
	{
		temp = ms->all_cmd_paths[i];
		ms->all_cmd_paths[i] = ft_strjoin(temp, "/");
		free(temp);
		temp = ms->all_cmd_paths[i];
		ms->all_cmd_paths[i] = ft_strjoin(temp, ms->cmd->args[0]);
		free(temp);
		i++;
	}
}
