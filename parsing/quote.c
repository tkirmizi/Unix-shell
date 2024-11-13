/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:38:36 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 12:39:23 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



char *handle_single_quotes(char *str, size_t *i, char *result, t_expansion *exp)
{
	char *temp;

	exp->in_squote = !exp->in_squote;
	(*i)++;
	while (str[*i] && str[*i] != '\'')
	{
		temp = ft_strnjoin(result, &str[*i], 1);
		if (!temp)
			return (NULL);
		result = temp;
		(*i)++;
	}
	if (str[*i] == '\'')
	{
		exp->in_squote = !exp->in_squote;
		(*i)++;
	}
	return (result);
}

char *handle_double_quotes(char *str, size_t *i, char *result, t_expansion *exp)
{
	char *temp;

	exp->in_dquote = !exp->in_dquote;
	(*i)++;
	while (str[*i] && str[*i] != '\"')
	{
		if (str[*i] == '$' && str[*i + 1])
		{
			result = handle_dollar(str, i, exp, &result);
			if (!result)
				return (NULL);
		}
		else
		{
			temp = ft_strnjoin(result, &str[*i], 1);
			if (!temp)
				return (NULL);
			result = temp;
			(*i)++;
		}
	}
	if (str[*i] == '\"')
		handle_double_q_cont(str, i, result, exp);
	return (result);
}

void	handle_double_q_cont(char *str, size_t *i, char *result, t_expansion *exp)
{
	exp->in_dquote = !exp->in_dquote;
	(*i)++;
}

char *handle_regular_char(char *str, size_t *i, char *result)
{
	char *temp;

	temp = ft_strnjoin(result, &str[*i], 1);
	if (!temp)
		return (NULL);
	(*i)++;
	return (temp);
}

int process_dbl_quotes(char *str, int *i, char **ex_str, t_ms *ms)
{
	(*i)++;
	while (str[*i] && str[*i] != '\"')
	{
		if (str[*i] == '$' && str[*i + 1] && str[*i + 1] != '\"' && str[*i + 1] != ' ')
		{
			process_var_expasion_first(str, i, ex_str, ms);
			(*i)--;
		}
		else
			*ex_str = ft_strnjoin(*ex_str, &str[*i], 1);
		(*i)++;
	}
	if (str[*i])
		(*i)++;
	return (1);
}