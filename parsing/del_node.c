/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:06:26 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/18 12:08:25 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	del_set_node_first(t_env **env_s, char **token, char **type)
{
	free(*token);
	free(*type);
	free_struct(env_s);
	exit_w_out_free("error: malloc", 1);
}

void	del_set_node_sec(t_env **env_s, char **token, char **type, t_env **node)
{
	free(node);
	free(*token);
	free(*type);
	free_struct(env_s);
	exit_w_out_free("error: malloc", 1);
}

void	del_set_node_tr(t_env **env_s, char **token, char **type, t_env **node)
{
	free((*node)->env_name);
	free(node);
	free(type);
	free(*token);
	free_struct(env_s);
	exit_w_out_free("error: malloc", 1);
}
