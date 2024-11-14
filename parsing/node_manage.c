/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_manage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:43:45 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 19:13:56 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_cmd_node(t_ms *mini, t_cmd *node_to_remove)
{
	int	i;

	if (mini == NULL || node_to_remove == NULL)
		return ;
	if (node_to_remove == mini->cmd)
		mini->cmd = node_to_remove->next;
	if (node_to_remove->prev != NULL)
		node_to_remove->prev->next = node_to_remove->next;
	if (node_to_remove->next != NULL)
		node_to_remove->next->prev = node_to_remove->prev;
	if (node_to_remove->fd_in != STDIN_FILENO)
		close(node_to_remove->fd_in);
	if (node_to_remove->fd_out != STDOUT_FILENO)
		close(node_to_remove->fd_out);
	if (node_to_remove->args != NULL)
	{
		i = 0;
		while (node_to_remove->args[i])
		{
			free(node_to_remove->args[i]);
			i++;
		}
		free(node_to_remove->args);
	}
	free(node_to_remove);
}

void	ft_del_array(char ***array_ptr, int index)
{
	char	**array;
	int		size;
	int		i;

	i = index;
	array = *array_ptr;
	size = 0;
	while (array[size] != NULL)
		size++;
	free(array[index]);
	while (i < size - 1)
	{
		array[i] = array[i + 1];
		i++;
	}
	array[size - 1] = NULL;
}
