/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:47:37 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 13:48:34 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void update_path(t_ms **ms)
{
	t_env *temp;
	int i;
	char **old_env;

	i = 0;
	temp = (*ms)->env_s;
	while (temp != NULL)
	{
		i++;
		temp = temp->next;
	}
	old_env = (*ms)->env;
	(*ms)->env = (char **)malloc((i + 1) * sizeof(char *));
	(*ms)->env[i] = NULL;
	temp = (*ms)->env_s;
	fill_array(ms, temp, 0);
}

void fill_array(t_ms **ms, t_env *temp, int i)
{
	char *tmp;

	while (temp != NULL)
	{
		(*ms)->env[i] = ft_strdup(temp->env_name);
		tmp = (*ms)->env[i];
		(*ms)->env[i] = ft_strjoin((*ms)->env[i], "=");
		free(tmp);
		tmp = (*ms)->env[i];
		(*ms)->env[i] = ft_strjoin((*ms)->env[i], temp->env_value);
		free(tmp);
		i++;
		temp = temp->next;
	}
}

void cleanup_env(t_env *env_s)
{
	t_env *current;
	t_env *next;

	current = env_s;
	while (current)
	{
		next = current->next;
		if (current->env_name)
			free(current->env_name);
		if (current->env_value)
			free(current->env_value);
		free(current);
		current = next;
	}
}

void ft_set_env(char *name, char *value, int overwrite, t_env *env_s)
{
	t_env *tmp;
	char *new_value;

	tmp = env_s;
	while (tmp)
	{
		if (ft_strncmp(tmp->env_name, name, ft_strlen(name)) == 0)
		{
			if (overwrite == 1)
			{
				new_value = ft_strdup(value);
				free(tmp->env_value);
				tmp->env_value = new_value;
			}
			return;
		}
		tmp = tmp->next;
	}
	ft_set_node(&tmp, name, value);
	env_s = tmp;
}