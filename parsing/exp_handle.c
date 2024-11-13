/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:36:33 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 12:52:32 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int expand(t_ms *ms)
{
	t_cmd *cmd;
	t_expansion *exp;
	int result;

	if (!ms || !ms->cmd)
		return (0);
	exp = init_expansion();
	if (!exp)
		return (0);
	exp->last_exit_code = ms->exit_code;
	cmd = ms->cmd;
	result = 1;
	while (cmd && result)
	{
		if (!cmd->args)
		{
			cmd = cmd->next;
			continue;
		}
		result = expand_args(cmd, exp);
		cmd = cmd->next;
	}
	destroy_expansion(exp);
	return (result);
}

int expand_args(t_cmd *cmd, t_expansion *exp)
{
	int i;
	char *expanded;
	int split_result;

	i = 0;
	while (cmd->args[i])
	{
		reset_expansion(exp);
		expanded = expand_variables(cmd->args[i], exp);
		if (!expanded)
			return (0);
		split_result = handle_expanded(cmd, expanded, i, exp);
		if (split_result == -1)
			return (0);
		i += split_result + 1;
	}
	return (1);
}

char *expand_variables(char *str, t_expansion *exp)
{
	char *result;
	size_t i;

	result = NULL;
	i = 0;
	while (str && str[i])
	{
		if (str[i] == '\'')
			result = handle_single_quotes(str, &i, result, exp);
		else if (str[i] == '\"')
			result = handle_double_quotes(str, &i, result, exp);
		else if (str[i] == '$' && str[i + 1])
			result = handle_dollar(str, &i, exp, &result);
		else
			result = handle_regular_char(str, &i, result);

		if (!result)
			return (NULL);
	}
	return (result);
}

void reset_expansion(t_expansion *exp)
{
	exp->result = NULL;
	exp->pos = 0;
	exp->in_squote = 0;
	exp->in_dquote = 0;
}

t_expansion *init_expansion(void)
{
	t_expansion *exp;

	exp = (t_expansion *)malloc(sizeof(t_expansion));
	if (!exp)
		return (NULL);

	exp->result = NULL;
	exp->pos = 0;
	exp->len = 0;
	exp->in_squote = 0;
	exp->in_dquote = 0;
	exp->last_exit_code = 0;
	exp->shell_pid = getpid();
	return (exp);
}