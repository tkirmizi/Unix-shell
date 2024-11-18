/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:47:37 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/18 11:18:36 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_path(t_ms **ms)
{
	t_env	*temp;
	int		i;

	i = 0;
	temp = (*ms)->env_s;
	while (temp != NULL)
	{
		i++;
		temp = temp->next;
	}
	(*ms)->env = (char **)malloc((i + 1) * sizeof(char *));
	if (!(*ms)->env)
		return ;
	(*ms)->env[i] = NULL;
	temp = (*ms)->env_s;
	fill_array(ms, temp, 0);
}

void	fill_array(t_ms **ms, t_env *temp, int i)
{
	char	*tmp1;
	char	*tmp2;

	while (temp != NULL)
	{
		tmp1 = ft_strdup(temp->env_name);
		if (!tmp1)
			return ;
		tmp2 = ft_strjoin(tmp1, "=");
		free(tmp1);
		if (!tmp2)
			return ;
		(*ms)->env[i] = ft_strjoin(tmp2, temp->env_value);
		free(tmp2);
		if (!(*ms)->env[i])
			return ;
		i++;
		temp = temp->next;
	}
}

void	cleanup_env(t_env *env_s)
{
	t_env	*temp;
	t_env	*next;

	temp = env_s;
	while (temp)
	{
		next = temp->next;
		if (temp->env_name)
			free(temp->env_name);
		if (temp->env_value)
			free(temp->env_value);
		free(temp);
		temp = next;
	}
}

void	ft_set_env(char *name, char *value, int overwrite, t_env *env_s)
{
	t_env	*tmp;
	char	*new_value;

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
			return ;
		}
		tmp = tmp->next;
	}
	ft_set_node(&tmp, name, value);
	env_s = tmp;
}
