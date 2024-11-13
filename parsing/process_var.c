/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:45:50 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 12:46:19 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int process_pid_exit(const char *str, int *i, char **ex_str, t_ms *ms)
{
	char *expansion;

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

int process_var_expasion_first(char *str, int *i, char **ex_str, t_ms *ms)
{
	if (*i < (int)ft_strlen(str))
		(*i)++;
	if (str[*i] == '$' || str[*i] == '?')
		return (process_pid_exit(str, i, ex_str, ms));
	ft_exp_concan(str, i, ex_str, ms);
	return (1);
}

int process_var_expasion_second(char *str, int *i, char **ex_str, t_ms *ms, int **s_flag)
{
	**s_flag = 1;

	if (*i < (int)ft_strlen(str))
		(*i)++;
	if (str[*i] == '$' || str[*i] == '?')
		return (process_pid_exit(str, i, ex_str, ms));
	ft_exp_concan(str, i, ex_str, ms);
	return (1);
}

int process_single_qutoes(char *str, int *i, char **ex_str)
{
	int start;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	*ex_str = ft_strnjoin(*ex_str, &str[start], *i - start);
	if (str[*i] == '\'')
		(*i)++;
	return (1);
}