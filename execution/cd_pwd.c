/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:45:23 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 13:46:09 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void do_cd(t_ms **ms)
{
	t_ms *temp;

	(*ms)->exit_code = 1;
	temp = (*ms);
	char *new_pwd;
	if (temp->cmd->args[2])
		printf("Only one path can be followed.\n");
	else if (!(ft_strncmp(temp->cmd->args[1], "-", 2)))
		new_pwd = find_last_part(ms);
	else if (!(ft_strncmp(temp->cmd->args[1], "..", 3)))
		new_pwd = find_prev_path(ms);
	else if (!(ft_strncmp(temp->cmd->args[1], "/", 2)))
		new_pwd = ft_strdup("/");
	else
		new_pwd = cur_pwd_return(ms);
	if (access(new_pwd, F_OK))
		printf("There is no such as path \n");
	else
	{
		change_pwd_oldpwd(ms);
		chdir_getcwd_all(ms, new_pwd);
		update_path(ms);
		(*ms)->exit_code = 0;
	}
}

void change_pwd_oldpwd(t_ms **ms)
{
	t_env *temp;

	char *pwd_wcpy;
	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "PWD", 3)))
			break;
		temp = temp->next;
	}
	pwd_wcpy = ft_strdup(temp->env_value);
	free(temp->env_value);
	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "OLDPWD", 6)))
			break;
		temp = temp->next;
	}
	free(temp->env_value);
	temp->env_value = ft_strdup(pwd_wcpy);
}

void chdir_getcwd_all(t_ms **ms, char *new_pwd)
{
	char *cur_pwd;
	int pwd_len;
	t_env *temp;

	temp = (*ms)->env_s;
	pwd_len = ft_strlen(new_pwd) + 1;
	cur_pwd = (char *)malloc((pwd_len) * sizeof(char));
	cur_pwd[pwd_len - 1] = '\0';
	if (chdir(new_pwd))
	{
		perror("Problem on changing directory\n");
		exit(1);
	}
	cur_pwd = getcwd(cur_pwd, pwd_len);
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "PWD", 3)))
			break;
		temp = temp->next;
	}
	temp->env_value = ft_strdup(cur_pwd);
}

char *find_prev_path(t_ms **ms)
{
	t_env *temp;
	char *new_pwd;
	char *wbtrim;

	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "PWD", 3)))
			break;
		temp = temp->next;
	}
	new_pwd = ft_strdup(temp->env_value);
	wbtrim = ft_strrchr(new_pwd, '/');
	*wbtrim = '\0';
	return (new_pwd);
}

char *find_last_part(t_ms **ms)
{
	t_env *temp;

	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "OLDPWD", 6)))
			break;
		temp = temp->next;
	}
	return (temp->env_value);
}