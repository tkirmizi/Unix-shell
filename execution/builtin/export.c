/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:56:02 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/09 10:56:26 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	do_export(t_ms **ms) // if just export without arguments what will happen ?
{
	int	i;
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


void	export_itself(t_ms **ms, char *string)
{
	t_env *temp;
	char *before_eq = NULL;
	char *after_eq;
	int	before_len;

	temp = (*ms)->env_s;
	after_eq = ft_strnstr(string, "=", ft_strlen(string));
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
	while (temp-> next != NULL)
	{
		if (!(ft_strncmp(temp->env_name, before_eq, before_len)))
		{
			free(temp->env_value);
			temp->env_value = ft_strdup(after_eq + 1);
			break;
		}
		temp = temp->next;
	}
	if (temp->next == NULL)
	{
		temp->next = (t_env*)malloc(sizeof(t_env));
		temp->next->env_name = ft_strdup(before_eq);
		temp->next->env_value = ft_strdup(after_eq + 1);
		temp->next->next = NULL;
	}
}


void	check_export_exist(t_ms **ms, char *string)
{
	t_env *temp;
	char *before_eq = NULL;
	char *after_eq;
	int	before_len;

	temp = (*ms)->env_s;
	after_eq = ft_strnstr(string, "=", ft_strlen(string));
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
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, before_eq, before_len)))
		{
			free(temp->env_value);
			temp->env_value = strdup("");
		}
		temp = temp->next;
	}
	free(before_eq);
}


void	update_path(t_ms **ms)
{
	t_env	*temp;
	int	i;

	i = 0;
	temp = (*ms)->env_s;
	while (temp != NULL)
	{
		i++;
		temp = temp->next;
	}
	(*ms)->env = (char**)malloc((i + 1) * sizeof(char *));
	(*ms)->env[i] = NULL;
	temp = (*ms)->env_s;
	i = 0;
	while (temp != NULL) // buraya niye girmiyor ?
	{
		(*ms)->env[i] = ft_strdup(temp->env_name);
		(*ms)->env[i] = ft_strjoin((*ms)->env[i], "=");
		(*ms)->env[i] = ft_strjoin((*ms)->env[i], temp->env_value);
		i++;
		temp = temp->next;
	}
}