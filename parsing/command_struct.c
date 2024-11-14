/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_struct.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:31:17 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 19:19:01 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_new_command(t_cmd **cmd)
{
	*cmd = malloc(sizeof(t_cmd));
	if (!*cmd)
	{
		write(2, "Memory allocation failed\n", 23);
		return (0);
	}
	(*cmd)->args = NULL;
	(*cmd)->next = NULL;
	(*cmd)->prev = NULL;
	(*cmd)->fd_in = STDIN_FILENO;
	(*cmd)->fd_out = STDOUT_FILENO;
	return (1);
}

int	setup_new_cmd(t_cmd **prev, t_token **token, t_cmd **cmd)
{
	t_cmd	*new;

	if (!init_new_command(&new))
		return (0);
	new->prev = *prev;
	if (*prev)
		(*prev)->next = new;
	if (!*cmd)
		*cmd = new;
	*prev = new;
	if ((*token)->prev && (*token)->prev->type != PIPE)
		*token = (*token)->next;
	return (1);
}

int	build_cmd_list(t_token *token, t_cmd **cmds)
{
	t_cmd	*cmd;

	cmd = NULL;
	while (token->type != END)
	{
		if ((!token->prev || token->prev->type == PIPE) && !setup_new_cmd(&cmd,
				&token, cmds))
			return (0);
		else if (token->type != PIPE && !extr_args(&token, cmd))
			return (0);
		else if (token->type == PIPE)
			token = token->next;
	}
	return (1);
}

int	commands(t_ms *ms)
{
	t_cmd	*result;

	result = NULL;
	if (!build_cmd_list(ms->token, &result))
		return (0);
	if (result)
		ms->cmd = result;
	return (1);
}

int	count_cmd_args(t_token *tok)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = tok;
	while (tmp && tmp->type != PIPE && tmp->type != END)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}
