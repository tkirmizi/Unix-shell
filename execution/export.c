/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:46:50 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 13:47:26 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void do_export(t_ms **ms)
{
	int i;
	t_ms *temp;

	(*ms)->exit_code = 1;
	temp = (*ms);
	i = 1;
	while (temp->cmd->args[i])
	{
		check_export_exist(ms, temp->cmd->args[i]);
		i++;
	}
	i = 1;
	temp = (*ms);
	while (temp->cmd->args[i])
	{
		export_itself(ms, temp->cmd->args[i]);
		i++;
	}
	update_path(ms);
	(*ms)->exit_code = 0;
}

void export_itself_cont(t_env *temp, char *before_eq, char *after_eq, int before_len)
{
	while (temp->next != NULL)
	{
		if (!(ft_strncmp(temp->env_name, before_eq, before_len)))
		{
			free(temp->env_value);
			temp->env_value = ft_strdup(after_eq + 1);
			return;
		}
		temp = temp->next;
	}
	temp->next = (t_env *)malloc(sizeof(t_env));
	temp->next->env_name = ft_strdup(before_eq);
	temp->next->env_value = ft_strdup(after_eq + 1);
	temp->next->next = NULL;
}

void export_itself(t_ms **ms, char *string)
{
	t_env *temp;
	char *before_eq;
	char *after_eq;
	int before_len;

	temp = (*ms)->env_s;
	after_eq = ft_strnstr(string, "=", ft_strlen(string));
	before_eq = NULL;
	if (after_eq != NULL)
	{
		before_len = after_eq - string;
		before_eq = malloc(before_len + 1);
		if (!before_eq)
		{
			perror("Failed to allocate memory");
			exit(1);
		}
		ft_strncpy(before_eq, string, before_len);
		before_eq[before_len] = '\0';
	}
	export_itself_cont(temp, before_eq, after_eq, before_len);
}

static void check_exp_exist_cont(t_env *temp, char *before_eq, int before_len)
{
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, before_eq, before_len)))
		{
			free(temp->env_value);
			temp->env_value = strdup("");
		}
		temp = temp->next;
	}
}

void check_export_exist(t_ms **ms, char *string)
{
	t_env *temp;
	char *before_eq;
	char *after_eq;
	int before_len;

	temp = (*ms)->env_s;
	after_eq = ft_strnstr(string, "=", ft_strlen(string));
	before_eq = NULL;
	if (after_eq != NULL)
	{
		before_len = after_eq - string;
		before_eq = malloc(before_len + 1);
		if (!before_eq)
		{
			perror("Failed to allocate memory");
			exit(1);
		}
		ft_strncpy(before_eq, string, before_len);
		before_eq[before_len] = '\0';
	}
	check_exp_exist_cont(temp, before_eq, before_len);
	free(before_eq);
}