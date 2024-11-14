/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:45:50 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 20:36:53 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	process_pid_exit(const char *str, int *i, char **ex_str, t_ms *ms)
{
	char	*expansion;

	(*i)++;
	if (str[*i] == '$')
	{
		expansion = ft_itoa((int)getpid());
		if (!expansion)
			return (0);
		*ex_str = ft_strnjoin(*ex_str, expansion, ft_strlen(expansion));
		free(expansion);
	}
	else if (str[*i] == '?')
	{
		expansion = ft_itoa(ms->exit_code);
		if (!expansion)
			return (0);
		*ex_str = ft_strnjoin(*ex_str, expansion, ft_strlen(expansion));
		free(expansion);
	}
	return (1);
}

int	process_var_expasion_first(char *str, int *i, char **ex_str, t_ms *ms)
{
	if (*i < (int)ft_strlen(str))
		(*i)++;
	if (str[*i] == '$' || str[*i] == '?')
		return (process_pid_exit(str, i, ex_str, ms));
	ft_exp_concan(str, i, ex_str, ms);
	return (1);
}

int	process_single_qutoes(char *str, int *i, char **ex_str)
{
	int	start;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	*ex_str = ft_strnjoin(*ex_str, &str[start], *i - start);
	if (str[*i] == '\'')
		(*i)++;
	return (1);
}

void	process_quote_handler(t_expand *exp, int *i)
{
	if (exp->str[*i] == '\'')
	{
		process_single_qutoes(exp->str, i, &exp->expanded_str);
		return ;
	}
	else if (exp->str[*i] == '\"')
	{
		process_dbl_quotes(exp->str, i, &exp->expanded_str, exp->ms);
		return ;
	}
	else if (exp->str[*i] == '$')
	{
		exp->i = i;
		process_var_expasion_second(exp);
	}
}

int	process_var_expasion_second(t_expand *exp)
{
	*exp->s_flag = 1;
	if (*exp->i < (int)ft_strlen(exp->str))
		(*exp->i)++;
	if (exp->str[*exp->i] == '$' || exp->str[*exp->i] == '?')
		return (process_pid_exit(exp->str, exp->i, &exp->expanded_str,
				exp->ms));
	ft_exp_concan(exp->str, exp->i, &exp->expanded_str, exp->ms);
	return (1);
}
