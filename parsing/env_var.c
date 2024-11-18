/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:40:27 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/18 12:07:28 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_fetch_env(const char *name, char **env)
{
	int	name_len;
	int	i;

	name_len = ft_strlen(name);
	i = 0;
	while (env && env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
		{
			return (&(env[i][name_len + 1]));
		}
		i++;
	}
	return (NULL);
}

t_env	*ft_save_env(char **env, t_env *env_struct)
{
	char	**env_split;
	int		i;

	i = 0;
	while (env[i])
	{
		env_split = ft_split(env[i], '=');
		if (env_split[0] && !env_split[1])
			env_split[1] = ft_strdup("");
		ft_set_node(&env_struct, env_split[0], env_split[1]);
		free(env_split);
		i++;
	}
	return (env_struct);
}

// void	ft_set_node(t_env **env_s, char *token, char *type)
// {
// 	t_env	*node;
// 	t_env	*last_node;

// 	if (NULL == env_s)
// 		return ;
// 	node = malloc(sizeof(t_env));
// 	if (!node)
// 	{
// 		free(token);
// 		free(type);
// 		free_struct(env_s);
// 		return ;
// 	}
// 	node->next = NULL;
// 	node->env_name = ft_strdup(token);
// 	if (!node->env_name)
// 	{
// 		free(node);
// 		free(token);
// 		free(type);
// 		free_struct(env_s);
// 		exit_w_out_free("error: malloc", 1);
// 	}
// 	free(token);
// 	if (type)
// 	{
// 		node->env_value = ft_strdup(type);
// 		if (!node->env_value)
// 		{
// 			free(node->env_name);
// 			free(node);
// 			free(type);
// 			free_struct(env_s);
// 			exit_w_out_free("error: malloc", 1);
// 		}
// 		free(type);
// 	}
// 	if (NULL == *env_s)
// 		*env_s = node;
// 	else
// 	{
// 		last_node = find_last(*env_s);
// 		last_node->next = node;
// 	}
// }

void	ft_set_node(t_env **env_s, char *token, char *type)
{
	t_env	*node;
	t_env	*last_node;

	node = malloc(sizeof(t_env));
	if (!node)
		del_set_node_first(env_s, &token, &type);
	node->next = NULL;
	node->env_name = ft_strdup(token);
	if (!node->env_name)
		del_set_node_sec(env_s, &token, &type, &node);
	free(token);
	if (type)
	{
		node->env_value = ft_strdup(type);
		if (!node->env_value)
			del_set_node_tr(env_s, &token, &type, &node);
		free(type);
	}
	if (NULL == *env_s)
		*env_s = node;
	else
	{
		last_node = find_last(*env_s);
		last_node->next = node;
	}
}

t_env_var	*create_env_var(const char *name, const char *value)
{
	t_env_var	*var;

	var = (t_env_var *)malloc(sizeof(t_env_var));
	if (!var)
		return (NULL);
	var->name = ft_strdup(name);
	var->value = ft_strdup(value);
	if (!var->name || !var->value)
	{
		free(var->name);
		free(var->value);
		free(var);
		return (NULL);
	}
	var->name_len = ft_strlen(name);
	var->value_len = ft_strlen(value);
	return (var);
}

void	destroy_env_var(t_env_var *var)
{
	if (!var)
		return ;
	free(var->name);
	free(var->value);
	free(var);
}
