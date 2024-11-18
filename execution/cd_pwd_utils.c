/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:46:16 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/15 11:38:20 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*cur_pwd_return(t_ms **ms)
{
	t_env	*temp;
	char	*new_pwd;
	char	*tmp;

	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "PWD", 3)))
			break ;
		temp = temp->next;
	}
	new_pwd = ft_strdup(temp->env_value);
	tmp = new_pwd;
	new_pwd = ft_strjoin(new_pwd, "/");
	free(tmp);
	tmp = new_pwd;
	new_pwd = ft_strjoin(new_pwd, (*ms)->cmd->args[1]);
	free(tmp);
	return (new_pwd);
}

void	do_pwd(t_ms **ms)
{
	t_env	*temp;
	int		len;

	(*ms)->exit_code = 1;
	temp = (*ms)->env_s;
	redir_for_builtin(ms);
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "PWD", 3)))
			break ;
		temp = temp->next;
	}
	len = ft_strlen(temp->env_value);
	write(STDOUT_FILENO, temp->env_value, len);
	write(STDOUT_FILENO, "\n", 1);
	(*ms)->exit_code = 0;
}
