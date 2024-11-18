/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_man.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:51:13 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/18 11:27:26 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	all_path_joiner(t_ms **ms, t_cmd **cmd)
{
	t_ms	*temp;

	temp = (*ms);
	while (temp->env_s)
	{
		if (ft_strncmp(temp->env_s->env_name, "PATH", 4) == 0)
			break ;
		temp->env_s = temp->env_s->next;
	}
	(*cmd)->path_for_excat = ft_split(temp->env_s->env_value, ':');
}

void	arg_join(t_cmd **cmd)
{
	int		i;
	char	*temp;
	t_cmd	*temp2;

	temp2 = (*cmd);
	i = 0;
	while (temp2->path_for_excat[i])
	{
		temp = temp2->path_for_excat[i];
		temp2->path_for_excat[i] = ft_strjoin(temp, "/");
		free(temp);
		temp = temp2->path_for_excat[i];
		temp2->path_for_excat[i] = ft_strjoin(temp, temp2->args[0]);
		free(temp);
		i++;
	}
}

void	find_exact_path(t_ms **ms, t_cmd **cmd, int *i)
{
	t_cmd	*temp;

	(*ms)->exit_code = 0;
	temp = (*cmd);
	*i = 0;
	while (temp->path_for_excat[*i])
	{
		if (access(temp->path_for_excat[*i], X_OK | F_OK) == 0)
			return ;
		(*i)++;
	}
	ft_write_to_fd(2, "command not found\n");
	exit(127);
}

void	update_shell_lvl(t_ms **ms)
{
	t_env	*temp;
	int		new_shlvl;
	char	*old_value;

	temp = (*ms)->env_s;
	while (temp)
	{
		if (ft_strncmp(temp->env_name, "SHLVL", 5) == 0)
			break ;
		temp = temp->next;
	}
	new_shlvl = ft_atoi(temp->env_value);
	new_shlvl += 1;
	old_value = temp->env_value;
	temp->env_value = ft_itoa(new_shlvl);
	free(old_value);
	update_path(ms);
}
