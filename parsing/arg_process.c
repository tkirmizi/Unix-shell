/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:32:35 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 12:33:23 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int store_args(t_cmd *cmd, t_token **tok, int size)
{
	int i;
	char *arg;

	i = 0;
	while (i < size)
	{
		if ((*tok)->type == WORD)
			arg = copy_token_value(*tok);
		else
			arg = copy_token_value(*tok);
		if (!arg)
		{
			clean_str_array(cmd->args, i);
			return (0);
		}
		cmd->args[i] = arg;
		*tok = (*tok)->next;
		i++;
	}
	cmd->args[i] = NULL;
	return (1);
}

int extr_args(t_token **token, t_cmd *cmd)
{
	int arg_count;

	arg_count = count_cmd_args(*token);
	if (arg_count == 0)
		return (1);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
	{
		write(2, "Memory allocation error\n", 22);
		return (0);
	}
	if (!store_args(cmd, token, arg_count))
		return (0);
	return (1);
}

char **ft_generate_args(char **new_args, char **args, char **splt_args,
						const int *n)
{
	int i;
	int j;
	int k;

	i = 0;
	k = 0;
	while (args[i])
	{
		if (i == *n)
		{
			j = 0;
			while (splt_args[j])
				new_args[k++] = splt_args[j++];
		}
		else
			new_args[k++] = args[i];
		i++;
	}
	new_args[k] = NULL;
	return (new_args);
}

char **allocate_new_args(char **args, const char *temp, char **split_args,
						 int *n)
{
	int i;
	int j;
	char **new_args;

	i = 0;
	while (args[i])
		i++;
	j = 0;
	while (split_args[j])
		j++;
	new_args = malloc(sizeof(char *) * (i + j));
	if (!new_args)
		return (NULL);
	new_args = ft_generate_args(new_args, args, split_args, n);
	return ((void)temp, new_args);
}

void remove_args(t_cmd *cmd)
{
	int i;

	i = 0;
	while (cmd->args[i])
	{
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
	cmd->args = NULL;
}