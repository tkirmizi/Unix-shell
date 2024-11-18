/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:44:44 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 22:08:31 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_split(t_cmd *cmd, char **split, int i, int original_count)
{
	t_split	*split_data;
	char	**new_args;
	size_t	new_size;

	split_data = (t_split *)malloc(sizeof(t_split));
	split_data->split_count = 0;
	while (split[split_data->split_count])
		split_data->split_count++;
	new_size = sizeof(char *) * (original_count + split_data->split_count + 1);
	new_args = ft_realloc_array(cmd->args, new_size);
	if (!new_args)
		return (0);
	cmd->args = new_args;
	free(cmd->args[i]);
	cmd->args[i] = ft_strdup(split[0]);
	if (split_data->split_count > 1)
		handle_split_cont(&split_data, split, &i, &original_count);
	return (split_data->split_count);
}

void	handle_split_cont(t_split **split_data, char **split, int *i,
		int *original_count)
{
	int		shift;
	int		j;
	t_split	*temp;

	temp = (*split_data);
	j = (*original_count) - 1;
	shift = (temp->split_count) - 1;
	while (j > (*i))
	{
		(temp->cmd)->args[j + shift] = (temp->cmd)->args[j];
		j--;
	}
	j = 1;
	while (j < (temp->split_count))
	{
		(temp->cmd)->args[(*i) + j] = ft_strdup(split[j]);
		j++;
	}
	(temp->cmd)->args[(*original_count) + shift] = NULL;
}

int	handle_split_args(char **split, t_cmd *cmd, int i, int org_count)
{
	int		split_count;
	char	**new_args;
	int		j;
	int		shift;
	size_t	new_size;

	split_count = 0;
	while (split[split_count])
		split_count++;
	new_size = sizeof(char *) * (org_count + split_count + 1);
	new_args = ft_realloc_array(cmd->args, new_size);
	if (!new_args)
		return (0);
	free(new_args[i]);
	new_args[i] = ft_strdup(split[0]);
	j = org_count;
	shift = split_count - 1;
	while (j > i + 1)
	{
		new_args[j + shift - 1] = new_args[j - 1];
		j--;
	}
	cmd->args = new_args;
	return (split_count);
}

int	handle_expanded(t_cmd *cmd, char *expanded, int i, t_expansion *exp)
{
	char	**split;
	int		original_count;
	int		split_count;
	int		j;

	j = -1;
	if (ft_strchr(expanded, ' ') && !exp->in_squote && !exp->in_dquote)
	{
		split = split_expanded(expanded);
		free(expanded);
		if (!split)
			return (-1);
		original_count = 0;
		while (cmd->args[original_count])
			original_count++;
		split_count = handle_split(cmd, split, i, original_count);
		while (split[++j])
			free(split[j]);
		free(split);
		if (!split_count)
			return (-1);
		return (split_count - 1);
	}
	handle_explanded_sec(&cmd, &expanded, &i);
	return (0);
}

int	handle_expanded_arg(char *expanded, t_cmd *cmd, int i, t_expansion *exp)
{
	char	**split;
	int		org_count;
	int		split_count;

	if (ft_strchr(expanded, ' ') && !exp->in_squote && !exp->in_dquote)
	{
		split = split_expanded(expanded);
		free(expanded);
		if (!split)
			return (0);
		org_count = 0;
		while (cmd->args[org_count])
			org_count++;
		split_count = handle_split_args(split, cmd, i, org_count);
		if (!split_count)
			return (0);
		return (copy_split_words(split, cmd->args, i));
	}
	free(cmd->args[i]);
	cmd->args[i] = expanded;
	return (0);
}
